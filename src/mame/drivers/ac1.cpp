// license:BSD-3-Clause
// copyright-holders:Miodrag Milanovic
/***************************************************************************

        AC1 video driver by Miodrag Milanovic

        15/01/2009 Preliminary driver.

        24/02/2011 Added cassette support ('ac1' and 'ac1_32' only)

        Note that Z command will get you into BASIC, and BYE command
        takes you back to the Monitor.

        S xxxx yyyy = to save memory to tape.
        L  = to load it back in.

        Since there is no motor control, type in L then mount the tape.

****************************************************************************/

#include "emu.h"
#include "includes/ac1.h"

#include "cpu/z80/z80.h"
#include "imagedev/cassette.h"
#include "machine/z80pio.h"
#include "sound/wave.h"
#include "screen.h"
#include "speaker.h"


static GFXDECODE_START( gfx_ac1 )
	GFXDECODE_ENTRY( "gfx1", 0x0000, ac1_charlayout, 0, 1 )
GFXDECODE_END

/* Address maps */
void ac1_state::ac1_mem(address_map &map)
{
	map.unmap_value_high();
	map(0x0000, 0x07ff).rom();  // Monitor
	map(0x0800, 0x0fff).rom();  // BASIC
	map(0x1000, 0x17ff).ram();  // Video RAM
	map(0x1800, 0x1fff).ram();  // RAM
}

void ac1_state::ac1_32_mem(address_map &map)
{
	map.unmap_value_high();
	map(0x0000, 0x07ff).rom();  // Monitor
	map(0x0800, 0x0fff).rom();  // BASIC
	map(0x1000, 0x17ff).ram();  // Video RAM
	map(0x1800, 0xffff).ram();  // RAM
}

void ac1_state::ac1_io(address_map &map)
{
	map.global_mask(0xff);
	map(0x04, 0x07).rw("z80pio", FUNC(z80pio_device::read), FUNC(z80pio_device::write));
}

/* Input ports */
static INPUT_PORTS_START( ac1 )
	PORT_START("LINE.0")
	PORT_BIT(0x01, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_0) PORT_CHAR('0')
	PORT_BIT(0x02, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_1) PORT_CHAR('1') PORT_CHAR('!')
	PORT_BIT(0x04, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_2) PORT_CHAR('2') PORT_CHAR('"')
	PORT_BIT(0x08, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_3) PORT_CHAR('3') PORT_CHAR('#')
	PORT_BIT(0x10, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_4) PORT_CHAR('4') PORT_CHAR('$')
	PORT_BIT(0x20, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_5) PORT_CHAR('5') PORT_CHAR('%')
	PORT_BIT(0x40, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_6) PORT_CHAR('6') PORT_CHAR('&')
	PORT_BIT(0x80, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_7) PORT_CHAR('7') PORT_CHAR('\'')

	PORT_START("LINE.1")
	PORT_BIT(0x01, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_8) PORT_CHAR('8') PORT_CHAR('(')
	PORT_BIT(0x02, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_9) PORT_CHAR('9') PORT_CHAR(')')
	PORT_BIT(0x04, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_COLON) PORT_CHAR(':') PORT_CHAR('*')
	PORT_BIT(0x08, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_OPENBRACE) PORT_CHAR(';') PORT_CHAR('+')
	PORT_BIT(0x10, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_COMMA) PORT_CHAR('<') PORT_CHAR(',')
	PORT_BIT(0x20, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_MINUS) PORT_CHAR('=') PORT_CHAR('-')
	PORT_BIT(0x40, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_STOP) PORT_CHAR('>') PORT_CHAR('.')
	PORT_BIT(0x80, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_SLASH) PORT_CHAR('?') PORT_CHAR('/')

	PORT_START("LINE.2")
	PORT_BIT(0x01, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_EQUALS) PORT_CHAR('@') PORT_CHAR('`')
	PORT_BIT(0x02, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_A) PORT_CHAR('A') PORT_CHAR('a')
	PORT_BIT(0x04, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_B) PORT_CHAR('B') PORT_CHAR('b')
	PORT_BIT(0x08, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_C) PORT_CHAR('C') PORT_CHAR('c')
	PORT_BIT(0x10, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_D) PORT_CHAR('D') PORT_CHAR('d')
	PORT_BIT(0x20, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_E) PORT_CHAR('E') PORT_CHAR('e')
	PORT_BIT(0x40, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_F) PORT_CHAR('F') PORT_CHAR('f')
	PORT_BIT(0x80, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_G) PORT_CHAR('G') PORT_CHAR('g')

	PORT_START("LINE.3")
	PORT_BIT(0x01, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_H) PORT_CHAR('H') PORT_CHAR('h')
	PORT_BIT(0x02, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_I) PORT_CHAR('I') PORT_CHAR('i')
	PORT_BIT(0x04, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_J) PORT_CHAR('J') PORT_CHAR('j')
	PORT_BIT(0x08, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_K) PORT_CHAR('K') PORT_CHAR('k')
	PORT_BIT(0x10, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_L) PORT_CHAR('L') PORT_CHAR('l')
	PORT_BIT(0x20, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_M) PORT_CHAR('M') PORT_CHAR('m')
	PORT_BIT(0x40, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_N) PORT_CHAR('N') PORT_CHAR('n')
	PORT_BIT(0x80, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_O) PORT_CHAR('O') PORT_CHAR('o')

	PORT_START("LINE.4")
	PORT_BIT(0x01, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_P) PORT_CHAR('P') PORT_CHAR('p')
	PORT_BIT(0x02, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_Q) PORT_CHAR('Q') PORT_CHAR('q')
	PORT_BIT(0x04, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_R) PORT_CHAR('R') PORT_CHAR('r')
	PORT_BIT(0x08, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_S) PORT_CHAR('S') PORT_CHAR('s')
	PORT_BIT(0x10, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_T) PORT_CHAR('T') PORT_CHAR('t')
	PORT_BIT(0x20, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_U) PORT_CHAR('U') PORT_CHAR('u')
	PORT_BIT(0x40, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_V) PORT_CHAR('V') PORT_CHAR('v')
	PORT_BIT(0x80, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_W) PORT_CHAR('W') PORT_CHAR('w')

	PORT_START("LINE.5")
	PORT_BIT(0x01, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_X) PORT_CHAR('X') PORT_CHAR('x')
	PORT_BIT(0x02, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_Y) PORT_CHAR('Y') PORT_CHAR('y')
	PORT_BIT(0x04, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_Z) PORT_CHAR('Z') PORT_CHAR('z')
	PORT_BIT(0x08, IP_ACTIVE_HIGH, IPT_UNUSED)
	PORT_BIT(0x10, IP_ACTIVE_HIGH, IPT_UNUSED)
	PORT_BIT(0x20, IP_ACTIVE_HIGH, IPT_UNUSED)
	PORT_BIT(0x40, IP_ACTIVE_HIGH, IPT_UNUSED)
	PORT_BIT(0x80, IP_ACTIVE_HIGH, IPT_UNUSED)

	PORT_START("LINE.6")
	PORT_BIT(0x01, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_NAME("Shift") PORT_CODE(KEYCODE_LSHIFT) PORT_CODE(KEYCODE_RSHIFT) PORT_CHAR(UCHAR_SHIFT_1)
	PORT_BIT(0x02, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_NAME("Ctrl")  PORT_CODE(KEYCODE_LCONTROL) PORT_CODE(KEYCODE_RCONTROL)
	PORT_BIT(0x04, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_SPACE) PORT_CHAR(' ')
	PORT_BIT(0x08, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_CODE(KEYCODE_ENTER) PORT_CHAR(13)
	PORT_BIT(0x10, IP_ACTIVE_HIGH, IPT_KEYBOARD) PORT_NAME("Backspace") PORT_CODE(KEYCODE_BACKSPACE) PORT_CHAR(8)
INPUT_PORTS_END

/* Machine driver */
MACHINE_CONFIG_START(ac1_state::ac1)
	/* basic machine hardware */
	MCFG_DEVICE_ADD("maincpu", Z80, XTAL(8'000'000) / 4)
	MCFG_DEVICE_PROGRAM_MAP(ac1_mem)
	MCFG_DEVICE_IO_MAP(ac1_io)

	MCFG_DEVICE_ADD("z80pio", Z80PIO, XTAL(8'000'000) / 4)
	MCFG_Z80PIO_IN_PA_CB(READ8(*this, ac1_state, ac1_port_a_r))
	MCFG_Z80PIO_OUT_PA_CB(WRITE8(*this, ac1_state, ac1_port_a_w))
	MCFG_Z80PIO_IN_PB_CB(READ8(*this, ac1_state, ac1_port_b_r))
	MCFG_Z80PIO_OUT_PB_CB(WRITE8(*this, ac1_state, ac1_port_b_w))

	/* video hardware */
	MCFG_SCREEN_ADD("screen", RASTER)
	MCFG_SCREEN_REFRESH_RATE(50)
	MCFG_SCREEN_VBLANK_TIME(ATTOSECONDS_IN_USEC(2500)) /* not accurate */
	MCFG_SCREEN_SIZE(64*6, 16*8)
	MCFG_SCREEN_VISIBLE_AREA(0, 64*6-1, 0, 16*8-1)
	MCFG_SCREEN_UPDATE_DRIVER(ac1_state, screen_update_ac1)
	MCFG_SCREEN_PALETTE("palette")

	MCFG_DEVICE_ADD("gfxdecode", GFXDECODE, "palette", gfx_ac1 )

	MCFG_PALETTE_ADD_MONOCHROME("palette")

	SPEAKER(config, "mono").front_center();
	WAVE(config, "wave", "cassette").add_route(ALL_OUTPUTS, "mono", 0.25);

	MCFG_CASSETTE_ADD( "cassette" )
MACHINE_CONFIG_END

MACHINE_CONFIG_START(ac1_state::ac1_32)
	ac1(config);

	MCFG_DEVICE_MODIFY("maincpu")
	MCFG_DEVICE_PROGRAM_MAP(ac1_32_mem)

	MCFG_SCREEN_MODIFY("screen")
	MCFG_SCREEN_SIZE(64*6, 32*8)
	MCFG_SCREEN_VISIBLE_AREA(0, 64*6-1, 0, 32*8-1)
	MCFG_SCREEN_UPDATE_DRIVER(ac1_state, screen_update_ac1_32)
MACHINE_CONFIG_END

/* ROM definition */
ROM_START( ac1 )
	ROM_REGION( 0x10000, "maincpu", ROMREGION_ERASEFF )
	ROM_SYSTEM_BIOS( 0, "v1", "Version 3.1 (orig)" )
	ROMX_LOAD( "mon_v31_16.bin",  0x0000, 0x0800, CRC(1ba65e4d) SHA1(3382b8d03f31166a56aea49fd1ec1e82a7108300), ROM_BIOS(1))
	ROM_SYSTEM_BIOS( 1, "v2", "Version 3.1 (fixed)" )
	ROMX_LOAD( "mon_v31_16_v2.bin",  0x0000, 0x0800, CRC(8904beb4) SHA1(db8d00a2537ac3a662e3c91e55eb2bf824a72062), ROM_BIOS(2))
	// from Funkamateur 01/85
	ROM_LOAD( "minibasic.bin",   0x0800, 0x0800, CRC(06782639) SHA1(3fd57b3ae3f538374b0d794d8aa15d06bcaaddd8))
	ROM_REGION(0x0800, "gfx1",0)
	// 64 chars - U402 BM513
	ROM_LOAD ("u402.bin", 0x0000, 0x0200, CRC(cfb67f28) SHA1(e3a62a3a8bce0d098887e31fd16410f38832fd18))
	ROM_COPY("gfx1", 0x0000, 0x0200, 0x0200)
	ROM_COPY("gfx1", 0x0000, 0x0400, 0x0200)
	ROM_COPY("gfx1", 0x0000, 0x0600, 0x0200)
ROM_END

ROM_START( ac1_32 )
	ROM_REGION( 0x10000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD( "mon_v31_32.bin",  0x0000, 0x0800, CRC(bea78b1a) SHA1(8a3e2ac2033aa0bb016be742cfea7e4b09c0813b))
	// from Funkamateur 01/85
	ROM_LOAD( "minibasic.bin",   0x0800, 0x0800, CRC(06782639) SHA1(3fd57b3ae3f538374b0d794d8aa15d06bcaaddd8))
	ROM_REGION(0x0800, "gfx1",0)
	ROM_SYSTEM_BIOS( 0, "128", "128 chars" )
	// 128 chars - U555 or 2708 from Funkamateur 06/86 128 including pseudo graphics
	ROMX_LOAD ("zg_128.bin", 0x0000, 0x0400, CRC(0a6f7796) SHA1(64d77639b1ea23f45b4bd38c251851acb2d03822), ROM_BIOS(1))
	ROMX_LOAD ("zg_128.bin", 0x0400, 0x0400, CRC(0a6f7796) SHA1(64d77639b1ea23f45b4bd38c251851acb2d03822), ROM_BIOS(1))
	ROM_SYSTEM_BIOS( 1, "256", "256 chars" )
	// 256 chars - 2716 from Computerclub Dessau  including pseudo graphics
	ROMX_LOAD ("zg_256.bin", 0x0000, 0x0800, CRC(b4171df5) SHA1(abdec4e00257f86b1a57e02b9c6b4d2df2a2a2db), ROM_BIOS(2))
ROM_END

ROM_START( ac1scch )
	ROM_REGION( 0x10000, "maincpu", ROMREGION_ERASEFF )
	ROM_SYSTEM_BIOS( 0, "v7", "Version 7" )
	ROMX_LOAD( "mon_v7.bin",  0x0000, 0x1000, CRC(fd17b0cf) SHA1(e47113025bd9dadc1522425e21703f43e584b00f),ROM_BIOS(1))
	ROM_SYSTEM_BIOS( 1, "v8", "Version 8" )
	ROMX_LOAD( "mon_v8.bin",  0x0000, 0x1000, CRC(5af68da5) SHA1(e760d4400b9c937e7e789d52b8ec975ff253a122),ROM_BIOS(2))
	ROM_SYSTEM_BIOS( 2, "v10", "Version 10" )
	ROMX_LOAD( "mon_v10.bin",  0x0000, 0x1000, CRC(f8e67ecb) SHA1(7953676fc8c22824ceff464c7177e9ac0343b8ce),ROM_BIOS(3))
	ROM_SYSTEM_BIOS( 3, "v1088", "Version 10/88" )
	ROMX_LOAD( "mon_v1088.bin",  0x0000, 0x1000, CRC(bbb0a6df) SHA1(de9389e142541a8b5ff238b59e98bf571c794bef),ROM_BIOS(4))
	ROM_REGION(0x0800, "gfx1",0)
	ROM_LOAD ("zg_scch.bin", 0x0000, 0x0800, CRC(fbfaf5da) SHA1(667568c5909e9a17675cf09dfbce2fc090c420ab))
ROM_END

/* Driver */
/*    YEAR  NAME     PARENT  COMPAT  MACHINE  INPUT  CLASS      INIT      COMPANY         FULLNAME                                 FLAGS */
COMP( 1984, ac1,     0,      0,      ac1,     ac1,   ac1_state, init_ac1, "Frank Heyder", "Amateurcomputer AC1 Berlin",            0 )
COMP( 1984, ac1_32,  ac1,    0,      ac1_32,  ac1,   ac1_state, init_ac1, "Frank Heyder", "Amateurcomputer AC1 Berlin (32 lines)", 0 )
COMP( 1984, ac1scch, ac1,    0,      ac1_32,  ac1,   ac1_state, init_ac1, "Frank Heyder", "Amateurcomputer AC1 SCCH",              0 )
