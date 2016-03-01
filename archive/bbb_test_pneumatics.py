import sys
sys.path.insert(0, '../acoustics/pinger_finder/')

import bbb
from bbb import boot


class State(object):
    """ State constants """
    OPEN = 0
    CLOSED = 1

# Pins for pneumatics
PEDO_PIN = 'P8_07'
GRABBER_PIN = 'P8_08'
DEFAULT_GRABBER_STATE = State.CLOSED

# Strings for printing
QUERY_HDL = ">> "


def main():
    # Display init sequence
    init()

    # Bring up user interface
    ui()

    # exit
    exit_app()


def init():
    print('-' * 50)
    print("-- Pneumatics App --")
    print('-' * 50)


def exit_app():
    print("Goodbye!!!")


def ui():
    # Load the pneumatics
    PN = Pneumatics()

    # Greet the user
    print("Welcome! Please enter a command below:")

    while True:
        # query user for input
        user_input = query(QUERY_HDL)

        # c ############################
        ########### user Class ##
        ################################
        if ('quit' == user_input) or ('q' == user_input):
            PN.close()
            break

        elif ('help' == user_input) or ('h' == user_input):
            print('-' * 50)
            usage()
            print('-' * 50)

        # c ############################
        ########### torpedo class ##
        ################################
        elif ('fire' == user_input) or ('f' == user_input):
            print("Firing torpedo!")
            PN.fire_torpedo()

        elif('grabber' == user_input) or ('g' == user_input):

            if PN.grabber_status == State.OPEN:
                print("Closing Grabber")
                PN.close_grabber()

            elif PN.grabber_status == State.CLOSED:
                print("Opening Grabber")
                PN.open_grabber()

            else:
                print("Hmm... grabber has no status.")

        # c ############################
        ########### utility cmd Class ##
        ################################
        elif 'EOF' == user_input:
            response(loc.curr, "End of input file reached")

        else:
            print("Not a recognized command! Try again.")


def query(hdl):
    return raw_input(hdl)


def usage():
    text = ["  (h)elp: Get help.\n",
            "  (q)uit: quit this program.\n",
            "  (s)tatus: prints data regarding pneumatics.\n",
            #"  c(o)nfig: Change Settings of the pneumatics system.\n",
            "  (f)ire: fire the torpedos.\n",
            "  (g)rabber: toggle the grabber state.\n"]
    print('\n'.join(text))


class Pneumatics:

    def __init__(self):
        # Instantiate pedo pin
        self.pedo = bbb.Port(PEDO_PIN)
        self.pedo.set_port_dir("out")

        # instantiate grabber pin
        self.grabber = bbb.Port(GRABBER_PIN)
        self.grabber.set_port_dir("out")
        self.grabber_status = DEFAULT_GRABBER_STATE

    def re_init(self):
        """
        """
        self.__init__()

    def close(self):
        """
        """
        self.pedo.close()
        self.grabber.close()

    def help(self):
        """ Prints a list of all available attribute and methods for (self)
        """

        print("You can call the following attributes: ")
        for item in self.__dict__.keys():
            print("  *.%s" % item)

        print("")
        print("You also have access to the following methods: ")
        for item in [method for method in dir(self) if callable(getattr(self, method))]:
            print("  *.%s()" % item)
        print("")

    #######################
    ## Universal Methods ##
    #######################
    def reset(self):
        """ Reset the config
        """
        self.pedo.write_to_port(0)
        self.grabber.write_to_port(0)

    ######################
    ## Torpedo Methods ##
    ######################
    def fire_torpedo(self):
        """ Fire the torpedo
        """
        self.pedo.write_to_port(1)
        self.pedo.write_to_port(0)

    #####################
    ## Grabber methods ##
    #####################
    def close_grabber(self):
        """ Close the grabber
        """
        self.grabber.write_to_port(1)
        self.grabber_status = State.CLOSED

    def open_grabber(self):
        """ Open the grabber
        """
        self.grabber.write_to_port(0)
        self.grabber_status = State.OPEN
