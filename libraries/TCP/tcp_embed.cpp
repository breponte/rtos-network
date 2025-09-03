#include "tcp_embed.hpp"
#include "sensors.hpp"

HandshakeState handleTCPState(HandshakeState state, EventTCP event) {
    switch (state)
    {
    case CLOSED:
        if (event == Send) {
            // TODO: Send SYN
            return SYN_SENT;
        } else {
            return LISTEN;
        }
        break;
    case LISTEN:
        if (event == SYN) {
            // TODO: Send SYN_ACK
            return SYN_RCVD;
        } else if (event == Send) {
            // TODO: Send SYN
            return SYN_SENT;
        }
    case SYN_SENT:
        if (event == Close) {
            return CLOSED;
        } else if (event == SYN) {
            // TODO: Send SYN_ACK
            return SYN_RCVD;
        } else if (event == SYN_ACK) {
            // TODO: Send ACK
            return ESTABLISHED;
        }
    case SYN_RCVD:
        if (event == ACK) {
            return ESTABLISHED;
        } else if (event == Close) {
            // TODO: Send FIN
            return FIN_WAIT_1;
        }
    case ESTABLISHED:
        if (event == Close) {
            // TODO: Send FIN
            return FIN_WAIT_1;
        } else if (event == FIN) {
            // TODO: Send ACK
            return CLOSE_WAIT;
        }
        // TODO: Send message
    case FIN_WAIT_1:
        if (event == ACK) {
            return FIN_WAIT_2;
        } else if (event == FIN_ACK) {
            // TODO: Send ACK
            return TIME_WAIT;
        } else if (event == FIN) {
            // TODO: Send ACK
            return CLOSING;
        }
    case FIN_WAIT_2:
        if (event == FIN) {
            // TODO: Send ACK
            return TIME_WAIT;
        }
    case CLOSING:
        if (event == ACK) {
            return TIME_WAIT;
        }
    case TIME_WAIT:
        // TODO: Timeout
    case CLOSE_WAIT:
        if (event == Close) {
            // TODO: Send FIN
            return LAST_ACK;
        }
    case LAST_ACK:
        if (event == ACK) {
            return CLOSED;
        }

    default:
        // either unrecognized state or stall on state given no valid input
        return state;
    }
}