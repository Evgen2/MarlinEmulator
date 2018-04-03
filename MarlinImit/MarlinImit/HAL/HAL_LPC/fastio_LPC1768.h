/* fastio_LPC1768.h */


#ifndef	_FASTIO_DUE_H
#define	_FASTIO_DUE_H

int _read(int io);
void _write(uint32_t io, int v);
void _set_pin_input(uint32_t  io);
void _set_pin_output(uint32_t io);

/// Read a pin
#define _READ(IO) _read(IO)
/// Write to a pin
#define _WRITE(IO, v)  _write(IO, v)

/// toggle a pin
#define _TOGGLE(IO)  _WRITE(IO, !READ(IO))

/// set pin as input
//#define _SET_INPUT(IO)  pmc_enable_periph_clk(g_APinDescription[IO].ulPeripheralId); \
//                       PIO_Configure(g_APinDescription[IO].pPort, PIO_INPUT, g_APinDescription[IO].ulPin, 0)
#define _SET_INPUT(IO) _set_pin_input(IO)

/// set pin as output
//#define _SET_OUTPUT(IO)  PIO_Configure(g_APinDescription[IO].pPort, PIO_OUTPUT_1, \
//                         g_APinDescription[IO].ulPin, g_APinDescription[IO].ulPinConfiguration)
#define _SET_OUTPUT(IO) _set_pin_output(IO);


/// set pin as input with pullup mode
#define _PULLUP(IO, v)  { pinMode(IO, (v!=LOW ? INPUT_PULLUP : INPUT)); }

/// set pin as input wrapper
#define SET_INPUT(IO)  _SET_INPUT(IO)
/// set pin as input with pullup wrapper
#define SET_INPUT_PULLUP(IO) do{ _SET_INPUT(IO); _PULLUP(IO, HIGH); }while(0)
/// set pin as output wrapper
#define SET_OUTPUT(IO)  do{ _SET_OUTPUT(IO); _WRITE(IO, LOW); }while(0)

/// check if pin is an input wrapper
#define GET_INPUT(IO)  _GET_INPUT(IO)
/// check if pin is an output wrapper
#define GET_OUTPUT(IO)  _GET_OUTPUT(IO)


/// Read a pin wrapper
#define READ(IO)  _READ(IO)
/// Write to a pin wrapper
#define WRITE(IO, v)  _WRITE(IO, v)
/// toggle a pin wrapper
#define TOGGLE(IO)  _TOGGLE(IO)
// Shorthand
#define OUT_WRITE(IO, v) { SET_OUTPUT(IO); WRITE(IO, v); }

#endif // _FASTIO_DUE_H
