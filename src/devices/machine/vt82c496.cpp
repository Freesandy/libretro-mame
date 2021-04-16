// license:BSD-3-Clause
// copyright-holders:Barry Rodewald
/*

    VIA VT82C496G "Green PC" system chipset

*/

#include "emu.h"
#include "vt82c496.h"

/***************************************************************************
    IMPLEMENTATION
***************************************************************************/

DEFINE_DEVICE_TYPE(VT82C496, vt82c496_device, "vt82c496", "VIA VT82C496 system chipset")


vt82c496_device::vt82c496_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: device_t(mconfig, VT82C496, tag, owner, clock), m_cpu(*this, finder_base::DUMMY_TAG), m_space(nullptr), m_ram(*this, finder_base::DUMMY_TAG), m_rom(*this, finder_base::DUMMY_TAG),
	  m_bios_c0_r(*this, "bios_c0_r"),
	  m_bios_c4_r(*this, "bios_c4_r"),
	  m_bios_c8_r(*this, "bios_c8_r"),
	  m_bios_cc_r(*this, "bios_cc_r"),
	  m_bios_d0_r(*this, "bios_d0_r"),
	  m_bios_d4_r(*this, "bios_d4_r"),
	  m_bios_d8_r(*this, "bios_d8_r"),
	  m_bios_dc_r(*this, "bios_dc_r"),
	  m_bios_e0_r(*this, "bios_e0_r"),
	  m_bios_f0_r(*this, "bios_f0_r"),
	  m_bios_c0_w(*this, "bios_c0_w"),
	  m_bios_c4_w(*this, "bios_c4_w"),
	  m_bios_c8_w(*this, "bios_c8_w"),
	  m_bios_cc_w(*this, "bios_cc_w"),
	  m_bios_d0_w(*this, "bios_d0_w"),
	  m_bios_d4_w(*this, "bios_d4_w"),
	  m_bios_d8_w(*this, "bios_d8_w"),
	  m_bios_dc_w(*this, "bios_dc_w"),
	  m_bios_e0_w(*this, "bios_e0_w"),
	  m_bios_f0_w(*this, "bios_f0_w"),
	  m_reg_select(0)
{
}

void vt82c496_device::device_start()
{
	/* get address space we are working on */
	m_space = &m_cpu->space(AS_PROGRAM);

	save_pointer(m_reg,"Registers",0x100);
}

void vt82c496_device::device_reset()
{
	memset(m_reg,0,0x100);

	// set up default ROM banking
	m_space->install_read_bank(0xc0000,0xc3fff,0,m_bios_c0_r);
	m_space->install_read_bank(0xc4000,0xc7fff,0,m_bios_c4_r);
	m_space->install_read_bank(0xc8000,0xcbfff,0,m_bios_c8_r);
	m_space->install_read_bank(0xcc000,0xcffff,0,m_bios_cc_r);
	m_space->install_read_bank(0xd0000,0xd3fff,0,m_bios_d0_r);
	m_space->install_read_bank(0xd4000,0xd7fff,0,m_bios_d4_r);
	m_space->install_read_bank(0xd8000,0xdbfff,0,m_bios_d8_r);
	m_space->install_read_bank(0xdc000,0xdffff,0,m_bios_dc_r);
	m_space->install_read_bank(0xe0000,0xeffff,0,m_bios_e0_r);
	m_space->install_read_bank(0xf0000,0xfffff,0,m_bios_f0_r);
	m_space->nop_write(0xc0000,0xfffff);
	m_bios_c0_r->set_base(m_rom);
	m_bios_c4_r->set_base(m_rom+0x4000/4);
	m_bios_c8_r->set_base(m_rom+0x8000/4);
	m_bios_cc_r->set_base(m_rom+0xc000/4);
	m_bios_d0_r->set_base(m_rom+0x10000/4);
	m_bios_d4_r->set_base(m_rom+0x14000/4);
	m_bios_d8_r->set_base(m_rom+0x18000/4);
	m_bios_dc_r->set_base(m_rom+0x1c000/4);
	m_bios_e0_r->set_base(m_rom+0x20000/4);
	m_bios_f0_r->set_base(m_rom+0x30000/4);
}

uint8_t vt82c496_device::read(offs_t offset)
{
	if(offset == 1)
		return m_reg[m_reg_select];
	return 0x00;
}

void vt82c496_device::write(offs_t offset, uint8_t data)
{
	if(offset == 0)
		m_reg_select = data;
	if(offset == 1)
	{
		m_reg[m_reg_select] = data;
		switch(m_reg_select)
		{
			case 0x30:
				update_mem_c0(data);
				break;
			case 0x31:
				update_mem_d0(data);
				break;
			case 0x32:
				update_mem_e0(data);
				break;
		}
	}
}

void vt82c496_device::update_mem_c0(uint8_t data)
{
	if(data & 0x80)
		m_bios_cc_r->set_base(m_ram->pointer()+0xcc000);
	else
		m_bios_cc_r->set_base(m_rom+0xc000/4);
	if(data & 0x40)
	{
		m_space->install_write_bank(0xcc000,0xcffff,0,m_bios_cc_w);
		m_bios_cc_w->set_base(m_ram->pointer()+0xcc000);
	}
	else
		m_space->nop_write(0xcc000,0xcffff);

	if(data & 0x20)
		m_bios_c8_r->set_base(m_ram->pointer()+0xc8000);
	else
		m_bios_c8_r->set_base(m_rom+0x8000/4);
	if(data & 0x10)
	{
		m_space->install_write_bank(0xc8000,0xcbfff,0,m_bios_c8_w);
		m_bios_c8_w->set_base(m_ram->pointer()+0xc8000);
	}
	else
		m_space->nop_write(0xc8000,0xcbfff);

	if(data & 0x08)
		m_bios_c4_r->set_base(m_ram->pointer()+0xc4000);
	else
		m_bios_c4_r->set_base(m_rom+0x4000/4);
	if(data & 0x04)
	{
		m_space->install_write_bank(0xc4000,0xc7fff,0,m_bios_c4_w);
		m_bios_c4_w->set_base(m_ram->pointer()+0xc4000);
	}
	else
		m_space->nop_write(0xc4000,0xc7fff);

	if(data & 0x02)
		m_bios_c0_r->set_base(m_ram->pointer()+0xc0000);
	else
		m_bios_c0_r->set_base(m_rom+0);
	if(data & 0x01)
	{
		m_space->install_write_bank(0xc0000,0xc3fff,0,m_bios_c0_w);
		m_bios_c0_w->set_base(m_ram->pointer()+0xc0000);
	}
	else
		m_space->nop_write(0xc0000,0xc3fff);
}

void vt82c496_device::update_mem_d0(uint8_t data)
{
	if(data & 0x80)
		m_bios_dc_r->set_base(m_ram->pointer()+0xdc000);
	else
		m_bios_dc_r->set_base(m_rom+0x1c000/4);
	if(data & 0x40)
	{
		m_space->install_write_bank(0xdc000,0xdffff,0,m_bios_dc_w);
		m_bios_dc_w->set_base(m_ram->pointer()+0xdc000);
	}
	else
		m_space->nop_write(0xdc000,0xdffff);

	if(data & 0x20)
		m_bios_d8_r->set_base(m_ram->pointer()+0xd8000);
	else
		m_bios_d8_r->set_base(m_rom+0x18000/4);
	if(data & 0x10)
	{
		m_space->install_write_bank(0xd8000,0xdbfff,0,m_bios_d8_w);
		m_bios_d8_w->set_base(m_ram->pointer()+0xd8000);
	}
	else
		m_space->nop_write(0xd8000,0xdbfff);

	if(data & 0x08)
		m_bios_d4_r->set_base(m_ram->pointer()+0xd4000);
	else
		m_bios_d4_r->set_base(m_rom+0x14000/4);
	if(data & 0x04)
	{
		m_space->install_write_bank(0xd4000,0xd7fff,0,m_bios_d4_w);
		m_bios_d4_w->set_base(m_ram->pointer()+0xd4000);
	}
	else
		m_space->nop_write(0xd4000,0xd7fff);

	if(data & 0x02)
		m_bios_d0_r->set_base(m_ram->pointer()+0xd0000);
	else
		m_bios_d0_r->set_base(m_rom+0x10000/4);
	if(data & 0x01)
	{
		m_space->install_write_bank(0xd0000,0xd3fff,0,m_bios_d0_w);
		m_bios_d0_w->set_base(m_ram->pointer()+0xd0000);
	}
	else
		m_space->nop_write(0xd0000,0xd3fff);
}

void vt82c496_device::update_mem_e0(uint8_t data)
{
	if(data & 0x80)
		m_bios_e0_r->set_base(m_ram->pointer()+0xe0000);
	else
		m_bios_e0_r->set_base(m_rom+0x20000/4);

	if(data & 0x40)
	{
		m_space->install_write_bank(0xe0000,0xeffff,0,m_bios_e0_w);
		m_bios_e0_w->set_base(m_ram->pointer()+0xe0000);
	}
	else
		m_space->nop_write(0xe0000,0xeffff);

	if(data & 0x20)
		m_bios_f0_r->set_base(m_ram->pointer()+0xf0000);
	else
		m_bios_f0_r->set_base(m_rom+0x30000/4);

	if(data & 0x10)
	{
		m_space->install_write_bank(0xf0000,0xfffff,0,m_bios_f0_w);
		m_bios_f0_w->set_base(m_ram->pointer()+0xf0000);
	}
	else
		m_space->nop_write(0xf0000,0xfffff);
}
