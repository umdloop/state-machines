###############################################################################
#  Project: UMDLoop
#  Repository: https://github.com/umdloop/state-machines
#  File: fsm_gen.py
###############################################################################
#  Purpose:
#
#     This tool generates a FSM skeleton from a *.dot graph description
#     using the tinyfsm C++ library.
###############################################################################
#  Usage: fsm_gen.py [-h] [-s] infile outdir
#
#  Generate C++ state machine from dotfile graph.
#
#  positional arguments:
#  infile       a *.dot file to be converted.
#  outdir       directory to output Events.hpp States.hpp States.cpp
#
#  optional arguments:
#    -h, --help   show this help message and exit
#    -s, --stubs  forces stub generation even when this tool would otherwise
#                 perform code generation.
###############################################################################
#  Change History:
#
#    Author      Date      Description of Change
#  ----------  --------  --------------------------------------------------
#  Ryan W.     10-26-19  Initial version.
###############################################################################

import pygraphviz as pgv
import argparse
import os

###############################################################################
#  Utils
###############################################################################

EVENTS_GUARD = 'EVENTS_HPP_'
STATES_GUARD = 'STATES_HPP_'

def process_graph(infile, outdir):
    G = pgv.AGraph(infile)

    events_header = os.path.join(outdir, 'Events.hpp')
    states_header = os.path.join(outdir, 'States.hpp')
    states_source = os.path.join(outdir, 'States.cpp')

    gen_events_header(G, events_header);
    gen_states_header(G, states_header);
    print(G.nodes())

def graph_events(G):
    events = set()
    for e in G.edges():
        label = e.attr.__getitem__('label')
        events.add(label2event(label))
    return events

def label2event(label):
    label = label.strip()
    if label[0].isdigit():
        return 'STATE_DELAY'
    else:
        return label.upper()

def header_comment():
    return '/* auto-generated by fsm_gen.py */\n'

def start_guard(guard):
    return f'#ifndef {guard}\n#define {guard}\n'

def end_guard(guard):
    return f'#endif /* {guard} */\n'

###############################################################################
#  Events.hpp Generation
###############################################################################

def gen_events_header(G, file):
    events = graph_events(G)

    with open(file, "w") as f:
        f.write(header_comment() + '\n')
        f.write(start_guard(EVENTS_GUARD) + '\n')
        f.write(events_includes() + '\n')
        f.write(base_event_definition() + '\n')
        for e in events:
            f.write(event_definition(e))
        f.write('\n' + end_guard(EVENTS_GUARD) + '\n')

def events_includes():
    return '#include "tinyfsm.hpp"\n'

def base_event_definition():
    return 'struct BASE_EVENT : tinyfsm::Event {};\n'

def event_definition(event):
    return f'struct {event} : BASE_EVENT {{}};\n'

###############################################################################
#  States.hpp Generation
###############################################################################

def gen_states_header(G, file):
    events = graph_events(G)

    with open(file, "w") as f:
        f.write(header_comment() + '\n')
        f.write(start_guard(STATES_GUARD) + '\n')
        f.write(states_header_includes() + '\n')
        f.write(base_state_definition(events) + '\n')
        f.write(end_guard(STATES_GUARD) + '\n')

def states_header_includes():
    return ('include "Events.hpp"\n' 
            'include "tinyfsm.hpp"\n')

def base_state_definition(events):
    base = ('class BASE_STATE : public tinyfsm::Fsm<BASE_STATE> {\n'
            'public:\n'
            '    void react(tinyfsm::Event const &) {};\n'
            '    virtual void entry(void) {};\n'
            '    virtual void exit(void) {};\n\n')
    for e in events:
        base += base_react(e)
    return base + '};\n'

def base_react(event):
    return f'    virtual void react({event} const &) {{}};\n'

###############################################################################
#  States.cpp Generation
###############################################################################

# TODO

###############################################################################
#  Main
###############################################################################

parser = argparse.ArgumentParser(description='Generate C++ state machine from dotfile graph.')
parser.add_argument('-s', '--stubs', action='store_true',
                    help=('forces stub generation even when this tool '
                          'would otherwise perform code generation.'))
parser.add_argument('infile', help='a *.dot file to be converted.')
parser.add_argument('outdir', help='directory to output Events.hpp States.hpp States.cpp')

args = parser.parse_args()

if not os.path.exists(args.infile):
    print(f'Input file not found: {args.infile}')
    exit(1)

if not os.path.exists(args.outdir):
    os.makedirs(args.outdir)

process_graph(args.infile, args.outdir)

