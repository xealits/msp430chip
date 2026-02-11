---
title: Prototype Parsable Datasheet for MSP430G2xx and MSP430F2xx
author: Alex Toldaiev
date: February 7, 2026
---

# Introduction

Testing an old idea: single-sourced description of chips.

* Make a very readable and writable and parseable document
for declarative description of the memory and registers:
  + Markdown and HTML generated with Pandoc.
* And generate C++ headers from the parseable version,
using very well fitting C++ constructions, so that the header-generation
is trivial and does not encode any actual C++ logic.
* It should also somehow maintain a good link between the generated doxygen docs and the original document.
Add URLs in doxygen. But with respect to what? It needs some good citation to an HTML document.

I use the `msp430g2553.h` header for the MSP430G2553 model,
and the Ti User Guide for MSP430F2xx and MSP430G2xx chips.

Aside of memory maps of common devices, it should be possible
to also declaratively describe:
* Concrete MSP430 chips. Just instantiate all the devices and Special Function Registers
with the corresponding Ti C header. This is done in `controllers.hpp` for MSP430G2553.
It would probably be better to declaratively describe it in Markdown-HTML.
* Chips come in different packages. Describe the pinout of the packages.
* The Launchpad boards. My board `MSP_EXP430G2` is hand-coded in `controllers.hpp`.
It is probably useful to write it up in Markdown-HTML too.

It may be handy to compile custom documents for individual chips or boards.
The custom document would combine only the present devices in a readable way.
To avoid browsing for the info on a specific device (TimerA or ADC10)
and the board pinout etc.

# Devices in MSP430 chips

The memory maps (register maps) of devices in Ti MSP430 chips.
The info is from the `msp430g2553.h` header.
But the maps describe generic devices, not how they show up in the MSP430G2553 chip itself.
In fact, the chip is hand-coded in the `controllers.hpp` header.

<div class="device_template" id="TimerA">
Device template name: <dfn class="cpp_name">TimerA</dfn>

TimerA device info summary. It is called `Timer0_A3` in the `msp430g2553.h` header.
So, it could be TimerA3. But the User Guide talks about TimerA.
That's the naming used here.

<details>
<summary>
Registers and sub-devices.
</summary>

<ul>
<li class="register" id="TimerA.Control">
Register type name: <dfn>Control</dfn>. Width: <span class="width">16</span>.
<details>
<summary>
Control register fields.
</summary>

<ul>
<li class="field"> <dfn>taifg</dfn>   <span class="offset">0</span> <span class="width">1</span> </li>
<li class="field"> <dfn>taie</dfn>    <span class="offset">1</span> <span class="width">1</span> </li>
<li class="field"> <dfn>clear</dfn>   <span class="offset">2</span> <span class="width">1</span> </li>
<li class="field"> <dfn>unused3</dfn> <span class="offset">3</span> <span class="width">1</span> </li>

<li class="field"><dfn>mode_control</dfn>
<span class="offset">4</span> <span class="width">2</span>
<details>
  <summary>Value options.</summary>
  <data class="value_option" value="0">MODE_STOP</data>
  <data class="value_option" value="1">MODE_UPTO</data>
  <data class="value_option" value="2">MODE_CONT</data>
  <data class="value_option" value="3">MODE_UPDN</data>
</details>
</li>

<li class="field"> <dfn>clock_divider</dfn>
<span class="offset">6</span> <span class="width">2</span>
<details>
  <summary>Value options.</summary>
  <data class="value_option" value="0">DIVIDE_0</data>
  <data class="value_option" value="1">DIVIDE_2</data>
  <data class="value_option" value="2">DIVIDE_4</data>
  <data class="value_option" value="3">DIVIDE_8</data>
</details>
</li>

<li class="field"> <dfn>input_clock</dfn>
<span class="offset">8</span> <span class="width">3</span>
<details>
  <summary>Value options.</summary>
  <data class="value_option" value="0">INPUT_CLOCK_TACLK</data>
  <data class="value_option" value="1">INPUT_CLOCK_ACLK</data>
  <data class="value_option" value="2">INPUT_CLOCK_SMCLK</data>
  <data class="value_option" value="3">INPUT_CLOCK_INCLK</data>
</details>
</li>

<li class="field"> <dfn>unused11</dfn>
<span class="offset">11</span> <span class="width">5</span>
</li>

</ul>
</details>
</li>

<li class="register" id="TimerA.CounterRegister">
Register type name: <dfn>CounterRegister</dfn>. Width: <span class="width">16</span>.
</li>

<li class="device_template" id="TimerA.CaptureCompareBlock">
Device template name: <dfn>CaptureCompareBlock</dfn>.

TimerA can have 2-3 identical Capture/Compare blocks.
(The TimerA devices have only 2 blocks in MSP430 models
that have two TimerA devices, 0 and 1.
<em>TODO: check in the User Guide if it is really true:
the header defines two TimerA with 3 CapCom blocks for MSP430G2553.</em>)
<details>
<summary>
Registers and subdevices of a Capture/Compare Block.
</summary>

<ul>
<li class="register" id="TimerA.CaptureCompareBlock.Control">
Register type name: <dfn>CapComControl</dfn>. Width: <span class="width">16</span>.
<details>
<summary>
Fields.
</summary>

<ul>
<li class="field"> <dfn>interrupt_flag</dfn>   <span class="offset">0</span> <span class="width">1</span> </li>
<li class="field"> <dfn>capture_overflow</dfn> <span class="offset">1</span> <span class="width">1</span> </li>
<li class="field"> <dfn>out_x</dfn>            <span class="offset">2</span> <span class="width">1</span> </li>
<li class="field"> <dfn>capcom_input</dfn>     <span class="offset">3</span> <span class="width">1</span> </li>

<li class="field"> <dfn>interrupt_enable</dfn>   <span class="offset">4</span> <span class="width">1</span> </li>
<li class="field"> <dfn>output_mode</dfn>        <span class="offset">5</span> <span class="width">3</span> </li>

<li class="field"> <dfn>capture_or_compare</dfn> <span class="offset">8</span> <span class="width">1</span> </li>
<li class="field"> <dfn>dummy_0</dfn>            <span class="offset">9</span> <span class="width">1</span> </li>
<li class="field"> <dfn>sync_capcom_input</dfn>  <span class="offset">10</span> <span class="width">1</span> </li>
<li class="field"> <dfn>async_sync_capture</dfn> <span class="offset">11</span> <span class="width">1</span> </li>

<li class="field"> <dfn>input_select</dfn>       <span class="offset">12</span> <span class="width">2</span> </li>
<li class="field"> <dfn>capture_mode</dfn>       <span class="offset">14</span> <span class="width">2</span> </li>
</ul>

</details>
</li>

<li class="register" id="TimerA.CaptureCompareBlock.Register">
Register type name: <dfn>CapComReg</dfn>.
</li>

</ul>

</details>
</li>

<li class="register" id="TimerA.InterruptVectorWord">
Register type name: <dfn>InterruptVectorWord</dfn>. Width: <span class="width">16</span>.
<em>TODO: check in the User Guide whether I got this one right.</em>
<details>
<summary>Fields.</summary>

<ul>
<li class="field"> <dfn>contents</dfn> <span class="offset">1</span> <span class="width">4</span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">NO_INTERRUPTS</data></span>
  <span class="value_option"><data value="1">CCR1_IFG</data></span>
  <span class="value_option"><data value="2">CCR2_IFG</data></span>
  <span class="value_option"><data value="5">TA_IFG</data></span>
</details>
</li>
</ul>
</details>

</li>
</ul>
</details>
</div>

Digital IO device, an 8-bit IO port:

<div class="device_template" id="PortIO8bit">
Device template name: <dfn class="cpp_name">PortIO8bit</dfn>

<span class="comment">
IO port control with <code>SEL</code> and <code>SEL2</code>.
Two SEL bits are described in slau144k, but not in the older slau208q.
SEL2 adds more peripheral functions to the port pins.
If SEL2 bit is set to 0b0, SEL works as usual (as in slau208q):
0b0 for I/O and 0b1 for the (primary) peripheral functions.
If SEL2 is set to 0b1, then SEL picks other peripheral functions:
0b0 for the secondary peripheral, 0b1 for the tertiary.

From slau208q and slau144k:
<q cite="slau208q">8.2.8 Configuring Unused Port Pins
Unused I/O pins should be configured as I/O function, output direction, and left unconnected on the PC board, to
prevent a floating input and reduce power consumption.
The value of the PxOUT bit is irrelevant, since the pin is unconnected.
Alternatively, the integrated pullup/pulldown resistor can be enabled by setting the PxREN bit of
the unused pin to prevent the floating input.
See the System Resets, Interrupts, and Operating Modes chapter for termination of unused pins.
</q>
The I/O function is selected by <code>sel</code> registers,
as in 12.2.6 Function Select Registers (PxSEL).

But slau144k has a note for MSP430G22x0 devices in 8.1 Digital I/O Introduction:
<q>
To avoid floating inputs on these GPIOs, these digital I/Os should be properly initialized by
running a start-up code similar to the following sample:
...
The initialization code configures GPIOs P1.0, P1.1, P1.3, and P1.4 as inputs with pulldown resistor
enabled (that is, P1REN.x = 1) and GPIOs P2.6 and P2.7 are terminated by selecting VLOCLK as
ACLK – see the Basic Clock System chapter for details...
</q>

I set the output direction for unused ports on Launchpad.
</span>

<details>
<summary>
Registers.
</summary>

<ul>
<li class="register" id="PortIO8bit.p_in"> Name: <dfn>p_in</dfn>. Width: <span class="width">8</span>. </li>
<li class="register" id="PortIO8bit.p_out"> Name: <dfn>p_out</dfn>. Width: <span class="width">8</span>. </li>
<li class="register" id="PortIO8bit.p_dir"> Name: <dfn>p_dir</dfn>. Width: <span class="width">8</span>.
<span class="comment"> 0b0 = input, 0b1 = output </span>
</li>

<li class="register" id="PortIO8bit.p_sel"> Name: <dfn>p_sel</dfn>. Width: <span class="width">8</span>.
<span class="comment">
  Port function selection:
  0b0 = I/O (or peripheral secondary)
  or
  0b1 = peripheral primary (or peripheral tertiary).
</span>
</li>

<li class="register" id="PortIO8bit.p_sel2"> Name: <dfn>p_sel2</dfn>. Width: <span class="width">8</span>.
<span class="comment">
  Two SEL bits are described in slau144k, but not in the older slau208q.
  SEL2 adds more peripheral functions to the port pins.
  If SEL2 bit is set to 0b0, SEL works as usual (as in slau208q):
  0b for I/O and 0b1 for the (primary) peripheral functions.
  If SEL2 is set to 0b1, then SEL picks other peripheral functions:
  0b for the secondary peripheral, 0b1 for the tertiary.
</span>
</li>

<li class="register" id="PortIO8bit.p_en"> Name: <dfn>p_en</dfn>. Width: <span class="width">8</span>.
<span class="comment">
  Port x pullup or pulldown resistor enable. When respective port is configured as
  input, setting this bit will enable the pullup or pulldown. <q cite="slau208q">See Table 12-1</q>
  0b = Pullup or pulldown disabled
  1b = Pullup or pulldown enabled
</span>
</li>
</ul>
</details>
</div>

Same 8-bit IO port with interrupts control:

<div class="device_template" id="PortIO8bitI">
Device template name: <dfn class="cpp_name">PortIO8bitI</dfn>

IO port control with interrupts.

<span class="comment">
IO port control with <code>SEL</code> and <code>SEL2</code>.
Two SEL bits are described in slau144k, but not in the older slau208q.
SEL2 adds more peripheral functions to the port pins.
If SEL2 bit is set to 0b0, SEL works as usual (as in slau208q):
0b0 for I/O and 0b1 for the (primary) peripheral functions.
If SEL2 is set to 0b1, then SEL picks other peripheral functions:
0b0 for the secondary peripheral, 0b1 for the tertiary.

From slau208q and slau144k:
<q cite="slau208q">8.2.8 Configuring Unused Port Pins
Unused I/O pins should be configured as I/O function, output direction, and left unconnected on the PC board, to
prevent a floating input and reduce power consumption.
The value of the PxOUT bit is irrelevant, since the pin is unconnected.
Alternatively, the integrated pullup/pulldown resistor can be enabled by setting the PxREN bit of
the unused pin to prevent the floating input.
See the System Resets, Interrupts, and Operating Modes chapter for termination of unused pins.
</q>
The I/O function is selected by <code>sel</code> registers,
as in 12.2.6 Function Select Registers (PxSEL).

But slau144k has a note for MSP430G22x0 devices in 8.1 Digital I/O Introduction:
<q>
To avoid floating inputs on these GPIOs, these digital I/Os should be properly initialized by
running a start-up code similar to the following sample:
...
The initialization code configures GPIOs P1.0, P1.1, P1.3, and P1.4 as inputs with pulldown resistor
enabled (that is, P1REN.x = 1) and GPIOs P2.6 and P2.7 are terminated by selecting VLOCLK as
ACLK – see the Basic Clock System chapter for details...
</q>

I set the output direction for unused ports on Launchpad.
</span>

<details>
<summary>
Registers.
</summary>

<ul>
<li class="register" id="PortIO8bitI.p_in"> Name: <dfn>p_in</dfn>. Width: <span class="width">8</span>. </li>
<li class="register" id="PortIO8bitI.p_out"> Name: <dfn>p_out</dfn>. Width: <span class="width">8</span>. </li>
<li class="register" id="PortIO8bitI.p_dir"> Name: <dfn>p_dir</dfn>. Width: <span class="width">8</span>.
<span class="comment"> 0b0 = input, 0b1 = output </span>
</li>

<li class="register" id="PortIO8bitI.p_sel"> Name: <dfn>p_sel</dfn>. Width: <span class="width">8</span>.
<span class="comment">
  Port function selection:
  0b0 I/O (or peripheral secondary)
  or
  0b1 peripheral primary (or peripheral tertiary).
</span>
</li>

<li class="register" id="PortIO8bitI.p_sel2"> Name: <dfn>p_sel2</dfn>. Width: <span class="width">8</span>.
<span class="comment">
  Two SEL bits are described in slau144k, but not in the older slau208q.
  SEL2 adds more peripheral functions to the port pins.

  If SEL2 bit is set to 0b0, SEL works as usual (as in slau208q):
  0b for I/O and 1b for the (primary) peripheral functions.
  If SEL2 is set to 0b1, then SEL picks other peripheral functions:
  0b for the secondary peripheral, 1b for the tertiary.
</span>
</li>

<li class="register" id="PortIO8bitI.p_en"> Name: <dfn>p_en</dfn>. Width: <span class="width">8</span>.
<span class="comment">
  Port x pullup or pulldown resistor enable. When respective port is configured as
  input, setting this bit will enable the pullup or pulldown. <q cite="slau208q">See Table 12-1</q>
  0b = Pullup or pulldown disabled
  1b = Pullup or pulldown enabled
</span>
</li>

<li class="register" id="PortIO8bitI.p_ifg"> Name: <dfn>p_ifg</dfn>. Width: <span class="width">8</span>. </li>
<li class="register" id="PortIO8bitI.p_ies"> Name: <dfn>p_ies</dfn>. Width: <span class="width">8</span>. </li>
<li class="register" id="PortIO8bitI.p_ie"> Name: <dfn>p_ie</dfn>. Width: <span class="width">8</span>. </li>
</ul>
</details>
</div>

A complicated one, ADC10:

<div class="device_template" id="ADC10">
Device template name: <dfn class="cpp_name">ADC10</dfn>

<details>
<summary>
Registers.
</summary>

<ul>
<li class="register" id="ADC10.DataTransferControl0">
Name: <dfn>DataTransferControl0</dfn>. Width: <span class="width">8</span>.
<details>
<summary>
Fields.
</summary>

<ul>
<li class="field"> <dfn>fetch</dfn> <span class="width">1</span>'@<span class="offset">0</span> </li>
<li class="field"> <dfn>block_one</dfn> <span class="width">1</span>'@<span class="offset">1</span> </li>
<li class="field"> <dfn>continuous_transfer</dfn> <span class="width">1</span>'@<span class="offset">2</span> </li>
<li class="field"> <dfn>two_block_mode</dfn> <span class="width">1</span>'@<span class="offset">3</span> </li>
</ul>
</details>
</li>

<li class="register" id="ADC10.DataTransferControl1">
Name: <dfn>DataTransferControl1</dfn>. Width: <span class="width">8</span>.
<details>
<summary>
Fields.
</summary>

<ul>
<li class="field"> <dfn>fetch</dfn> <span class="width">1</span>'@<span class="offset">0</span> </li>
<li class="field"> <dfn>block_one</dfn> <span class="width">1</span>'@<span class="offset">1</span> </li>
<li class="field"> <dfn>continuous_transfer</dfn> <span class="width">1</span>'@<span class="offset">2</span> </li>
<li class="field"> <dfn>two_block_mode</dfn> <span class="width">1</span>'@<span class="offset">3</span> </li>
</ul>
</details>
</li>


<li class="register" id="ADC10.AnalogEnable0">
Name: <dfn>AnalogEnable0</dfn>. Width: <span class="width">8</span>.
</li>

<li class="register" id="ADC10.Control0">
Name: <dfn>Control0</dfn>. Width: <span class="width">16</span>.
<details>
<summary>
Fields.
</summary>

<ul>
<li class="field"> <dfn>start_conversion</dfn> <span class="width">1</span>'@<span class="offset">0</span> </li>
<li class="field"> <dfn>enable_conversion</dfn> <span class="width">1</span>'@<span class="offset">1</span> </li>
<li class="field"> <dfn>interrupt_flag</dfn> <span class="width">1</span>'@<span class="offset">2</span> </li>
<li class="field"> <dfn>interrupt_enable</dfn> <span class="width">1</span>'@<span class="offset">3</span> </li>
<li class="field"> <dfn>on_enable</dfn> <span class="width">1</span>'@<span class="offset">4</span> </li>
<li class="field"> <dfn>ref_on</dfn> <span class="width">1</span>'@<span class="offset">5</span> </li>
<li class="field"> <dfn>ref_setting</dfn> <span class="width">1</span>'@<span class="offset">6</span>
0 = 1.5V, 1 = 2.5V
</li>
<li class="field"> <dfn>multiple_sample_conversion</dfn> <span class="width">1</span>'@<span class="offset">7</span> </li>

<li class="field"> <dfn>ref_burst_mode</dfn> <span class="width">1</span>'@<span class="offset">8</span> </li>
<li class="field"> <dfn>enable_output_of_ref</dfn> <span class="width">1</span>'@<span class="offset">9</span> </li>
<li class="field"> <dfn>sampling_rate</dfn> <span class="width">1</span>'@<span class="offset">10</span>
0 = 200ksps, 1 = 50ksps
</li>

<li class="field"> <dfn>sample_hold_select</dfn> <span class="width">2</span>'@<span class="offset">11</span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">SH_x4</data> <span class="comment">4 x ADC10 clocks</span></span>
  <span class="value_option"><data value="1">SH_x8</data> <span class="comment">8 x ADC10 clocks</span></span>
  <span class="value_option"><data value="2">SH_x16</data> <span class="comment">16 x ADC10 clocks</span></span>
  <span class="value_option"><data value="3">SH_x64</data> <span class="comment">64 x ADC10 clocks</span></span>
</details>
</li>

<li class="field"> <dfn>reference_select</dfn> <span class="width">3</span>'@<span class="offset">13</span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">REF_0</data> <span class="comment">VR+ = AVCC and VR- = AVSS</span></span>
  <span class="value_option"><data value="1">REF_1</data> <span class="comment">VR+ = VREF+ and VR- = AVSS</span></span>
  <span class="value_option"><data value="2">REF_2</data> <span class="comment">VR+ = VEREF+ and VR- = AVSS</span></span>
  <span class="value_option"><data value="3">REF_3</data> <span class="comment">VR+ = VEREF+ and VR- = AVSS</span></span>
  <span class="value_option"><data value="4">REF_4</data> <span class="comment">VR+ = AVCC and VR- = VREF-/VEREF-</span></span>
  <span class="value_option"><data value="5">REF_5</data> <span class="comment">VR+ = VREF+ and VR- = VREF-/VEREF-</span></span>
  <span class="value_option"><data value="6">REF_6</data> <span class="comment">VR+ = VEREF+ and VR- = VREF-/VEREF-</span></span>
  <span class="value_option"><data value="7">REF_7</data> <span class="comment">VR+ = VEREF+ and VR- = VREF-/VEREF-</span></span>
</details>
</li>

</ul>
</details>
</li>

<li class="register" id="ADC10.Control1">
Name: <dfn>Control1</dfn>. Width: <span class="width">16</span>.
Selects for: conversion sequence, clock source, clock divider, etc.
<details>
<summary>
Fields.
</summary>

<ul>
<li class="field"> <dfn>busy</dfn> <span class="width">1</span>'@<span class="offset">0</span> </li>
<li class="field"> <dfn>conversion_sequence</dfn> <span class="width">2</span>'@<span class="offset">1</span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">SingleChannelSingleConversion</data> </span>
  <span class="value_option"><data value="1">SequenceOfChannels</data> </span>
  <span class="value_option"><data value="2">RepeatSingleChannel</data> </span>
  <span class="value_option"><data value="3">RepeatSequenceOfChannels</data> </span>
</details>
</li>

<li class="field"> <dfn>clock_source</dfn> <span class="width">2</span>'@<span class="offset">3</span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">ADC10OSC</data> </span>
  <span class="value_option"><data value="1">ACLK</data> </span>
  <span class="value_option"><data value="2">MCLK</data> </span>
  <span class="value_option"><data value="3">SMCLK</data> </span>
</details>
</li>

<li class="field"> <dfn>clock_divider</dfn> <span class="width">3</span>'@<span class="offset">5</span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">DIV_0</data> </span>
  <span class="value_option"><data value="1">DIV_1</data> </span>
  <span class="value_option"><data value="2">DIV_2</data> </span>
  <span class="value_option"><data value="3">DIV_3</data> </span>
  <span class="value_option"><data value="4">DIV_4</data> </span>
  <span class="value_option"><data value="5">DIV_5</data> </span>
  <span class="value_option"><data value="6">DIV_6</data> </span>
  <span class="value_option"><data value="7">DIV_7</data> </span>
</details>
</li>

<li class="field"> <dfn>invert_sample_hold</dfn> <span class="width">1</span>'@<span class="offset">8</span> </li>
<li class="field"> <dfn>data_format</dfn> <span class="width">1</span>'@<span class="offset">9</span>
  <span class="value_option"><data value="0">BINARY</data> </span>
  <span class="value_option"><data value="1">TWOs_COMPLEMENT</data> </span>
  <span class="comment">0 = binary, 1 = 2's complement</span>
</li>
<li class="field"> <dfn>sample_hold_source</dfn> <span class="width">2</span>'@<span class="offset">10</span>
<details>
  <summary>Value options.</summary>
  For some reason the order is a bit weird: <code>TA3_OUT0=2</code> and <code>TA3_OUT2=3</code>.
  But that's how it is in the <code>msp430g2553.h</code> header.

  <span class="value_option"><data value="0">ADC10OSC</data> </span>
  <span class="value_option"><data value="1">TA3_OUT1</data> </span>
  <span class="value_option"><data value="2">TA3_OUT0</data> </span>
  <span class="value_option"><data value="3">TA3_OUT2</data> </span>
</details>
</li>

<li class="field"> <dfn>input_channel</dfn> <span class="width">4</span>'@<span class="offset">12</span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">CH_0</data> </span>
  <span class="value_option"><data value="1">CH_1</data> </span>
  <span class="value_option"><data value="2">CH_2</data> </span>
  <span class="value_option"><data value="3">CH_3</data> </span>
  <span class="value_option"><data value="4">CH_4</data> </span>
  <span class="value_option"><data value="5">CH_5</data> </span>
  <span class="value_option"><data value="6">CH_6</data> </span>
  <span class="value_option"><data value="7">CH_7</data> </span>

  <span class="value_option"><data value="8">CH_8</data> </span>
  <span class="value_option"><data value="9">CH_9</data> </span>
  <span class="value_option"><data value="10">CH_10</data> </span>
  <span class="value_option"><data value="11">CH_11</data> </span>
  <span class="value_option"><data value="12">CH_12</data> </span>
  <span class="value_option"><data value="13">CH_13</data> </span>
  <span class="value_option"><data value="14">CH_14</data> </span>
  <span class="value_option"><data value="15">CH_15</data> </span>
</details>
</li>

</ul>
</details>
</li>

</ul>
</details>
</div>


Basic Clock Module:

<div class="device_template" id="ADC10">
Device template name: <dfn class="cpp_name">BasicClockModule</dfn>

<details>
<summary>
Registers.
</summary>

<ul>
<li class="register" id="BasicClockModule.DCOClockFrequencyControl">
Name: <dfn>DCOClockFrequencyControl</dfn>. Width: <span class="width">8</span>.
5 bits for modulation, 3 bits for DCO select.
<details>
<summary>
Fields.
</summary>

<ul>
<li class="field"> <dfn>Modulation</dfn> <span class="width">5</span>'@<span class="offset">0</span> </li>
<li class="field"> <dfn>DCOSelect</dfn> <span class="width">3</span>'@<span class="offset">5</span> </li>
</ul>
</details>
</li>

<li class="register" id="BasicClockModule.Control1">
Basic Clock System control: <dfn>Control1</dfn>. Width: <span class="width">8</span>.
<details>
<summary>
Fields.
</summary>

<ul>
<li class="field"> <dfn>Range</dfn> <span class="width">4</span>'@<span class="offset">0</span> </li>
<li class="field"> <dfn>ACLKDivider</dfn> <span class="width">2</span>'@<span class="offset">4</span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">DIV_x1</data> </span>
  <span class="value_option"><data value="1">DIV_x2</data> </span>
  <span class="value_option"><data value="2">DIV_x4</data> </span>
  <span class="value_option"><data value="3">DIV_x8</data> </span>
</details>
</li>

<li class="field"> <dfn>XTSelect</dfn> <span class="width">1</span>'@<span class="offset">6</span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">LOW_FREQUENCY</data> </span>
  <span class="value_option"><data value="1">HIGH_FREQUENCY</data> </span>
</details>
</li>
<li class="field"> <dfn>XT2Off</dfn> <span class="width">1</span>'@<span class="offset">7</span>
  <span class="comment">Enable XT2CLK</span>
</li>
</ul>
</details>
</li>

<li class="register" id="BasicClockModule.Control2">
Basic Clock System control: <dfn>Control2</dfn>. Width: <span class="width">8</span>.
<details>
<summary>
Fields.
</summary>

<ul>
<li class="field"> <dfn>SMCLKDivider</dfn> <span class="width">2</span>'@<span class="offset">0</span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">DIV_x1</data> </span>
  <span class="value_option"><data value="1">DIV_x2</data> </span>
  <span class="value_option"><data value="2">DIV_x4</data> </span>
  <span class="value_option"><data value="3">DIV_x8</data> </span>
</details>
</li>

<li class="field"> <dfn>SMCLKSource</dfn> <span class="width">1</span>'@<span class="offset">2</span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">DCOCLK</data> </span>
  <span class="value_option"><data value="1">XT2CLK_LFXTCLK</data> </span>
</details>
</li>

<li class="field"> <dfn>MCLKDivider</dfn> <span class="width">2</span>'@<span class="offset">3</span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">DIV_x1</data> </span>
  <span class="value_option"><data value="1">DIV_x2</data> </span>
  <span class="value_option"><data value="2">DIV_x4</data> </span>
  <span class="value_option"><data value="3">DIV_x8</data> </span>
</details>
</li>

<li class="field"> <dfn>MCLKSource</dfn> <span class="width">2</span>'@<span class="offset">5</span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">DCOCLK</data> </span>
  <span class="value_option"><data value="1">DCOCLK</data> </span>
  <span class="value_option"><data value="2">XT2CLK_LFXTCLK</data> </span>
  <span class="value_option"><data value="3">LFXTCLK</data> </span>
</details>
</li>
</ul>

</details>
</li>

<li class="register" id="BasicClockModule.Control3">
Basic Clock System control: <dfn>Control3</dfn>. Width: <span class="width">8</span>.
<details>
<summary>
Fields.
</summary>

<ul>
<li class="field"> <dfn>LFXT_OscillatorFaultFlag</dfn> <span class="width">1</span>'@<span class="offset">0</span>
  <span class="comment">Low/high frequency oscillator fault flag</span>
</li>
<li class="field"> <dfn>XT_OscillatorFaultFlag</dfn> <span class="width">1</span>'@<span class="offset">1</span>
  <span class="comment">High frequency oscillator 2 fault flag</span>
</li>

<li class="field"> <dfn>XIN_XOUT_Cap</dfn> <span class="width">2</span>'@<span class="offset">2</span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">XCAP_0pF</data> </span>
  <span class="value_option"><data value="1">XCAP_6pF</data> </span>
  <span class="value_option"><data value="2">XCAP_10pF</data> </span>
  <span class="value_option"><data value="3">XCAP_12pF5</data> </span>
</details>
</li>

<li class="field"> <dfn>LFXT1_Mode</dfn> <span class="width">2</span>'@<span class="offset">4</span>
<span class="comment">(XTS = 0)</span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">NORMAL_OPERATION</data> </span>
  <span class="value_option"><data value="1">Reserved</data> </span>
  <span class="value_option"><data value="2">VLO</data> </span>
  <span class="value_option"><data value="3">DIGITAL_INPUT_SIGNAL</data> </span>
</details>
</li>

<li class="field"> <dfn>XT2_Mode</dfn> <span class="width">2</span>'@<span class="offset">6</span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">MODE_0p4_1_MHz</data> </span>
  <span class="value_option"><data value="1">MODE_1_4_MHz</data> </span>
  <span class="value_option"><data value="2">MODE_2_16_MHz</data> </span>
  <span class="value_option"><data value="3">DIGITAL_INPUT_SIGNAL</data> </span>
</details>
</li>

</ul>
</details>
</li>

</ul>
</details>
</div>

Comparator A:

<div class="device_template" id="ComparatorA">
Device template name: <dfn class="cpp_name">ComparatorA</dfn>

<details>
<summary>
Registers.
</summary>

<ul>
<li class="register" id="ComparatorA.Control1">
Name: <dfn>Control1</dfn>. Width: <span class="width">8</span>.
<details>
<summary>
Fields.
</summary>

<ul>
<li class="field"> <dfn>InterruptFlag</dfn> <span class="width">1</span>'@<span class="offset">0</span> </li>
<li class="field"> <dfn>InterruptEnable</dfn> <span class="width">1</span>'@<span class="offset">1</span> </li>
<li class="field"> <dfn>EdgeSelect</dfn> <span class="width">1</span>'@<span class="offset">2</span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">RISING</data> </span>
  <span class="value_option"><data value="1">FALLING</data> </span>
</details>
</li>

<li class="field"> <dfn>Enable</dfn> <span class="width">1</span>'@<span class="offset">3</span> </li>

<li class="field"> <dfn>InternalReference</dfn> <span class="width">2</span>'@<span class="offset">4</span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">OFF</data> </span>
  <span class="value_option"><data value="1">REF_0p25Vcc</data> </span>
  <span class="value_option"><data value="2">REF_0p5Vcc</data> </span>
  <span class="value_option"><data value="3">REF_Vt</data> </span>
</details>
</li>

<li class="field"> <dfn>InternalReferenceEnable</dfn> <span class="width">1</span>'@<span class="offset">6</span> </li>
<li class="field"> <dfn>ExchangeInputs</dfn> <span class="width">1</span>'@<span class="offset">7</span> </li>

</ul>
</details>
</li>

<li class="register" id="ComparatorA.Control2">
Name: <dfn>Control2</dfn>. Width: <span class="width">8</span>.
<details>
<summary>
Fields.
</summary>

<ul>
<li class="field"> <dfn>ComparatorAOutput</dfn> <span class="width">1</span>'@<span class="offset">0</span> </li>
<li class="field"> <dfn>EnableOutputFilter</dfn> <span class="width">1</span>'@<span class="offset">1</span> </li>
<li class="field"> <dfn>TerminalMultiplexer</dfn> <span class="width">5</span>'@<span class="offset">2</span> </li>
<li class="field"> <dfn>ShortTerminals</dfn> <span class="width">1</span>'@<span class="offset">7</span> </li>
</ul>
</details>
</li>

<li class="register" id="ComparatorA.PortDisable">
Name: <dfn>PortDisable</dfn>. Width: <span class="width">8</span>.
<details>
<summary>
Fields.
</summary>

<ul>
<li class="field"> <dfn>InputBuffersOfPortRegister</dfn> <span class="width">8</span>'@<span class="offset">0</span> </li>
</ul>
</details>
</li>

</ul>
</details>
</div>

Flash memory.
Somehow the User Guide (slau208q) misses the `Control2` register, instead it has a `Control4`.
And it mentions the Flash memory access violation interrupt enable in `SFRIE1`,
which I do not find in the `msp430g2553.h` header.
Therefore I am currently making two devices for both options:
the 3-registers one and the 4-registers.

* This should be done with composition, i.e. optional register. Or with inheritance.

<div class="device_template" id="FlashMemoryModule">
Device template name: <dfn class="cpp_name">FlashMemoryModule</dfn>

<details>
<summary>
Registers.
</summary>

<ul>
<li class="register" id="FlashMemoryModule.Control1">
Name: <dfn>Control1</dfn>. Width: <span class="width">16</span>.
<details>
<summary>
Fields.
</summary>

<ul>
<li class="field"> <dfn>Reserved0</dfn> <span class="width">1</span>'@<span class="offset">0</span> </li>
<li class="field"> <dfn>Erase_MassErase</dfn> <span class="width">2</span>'@<span class="offset">1</span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">NO_ERASE</data> </span>
  <span class="value_option"><data value="1">SEGMENT_ERASE</data> </span>
  <span class="value_option"><data value="2">BANK_ERASE</data> <span class="comment">erase one bank</span> </span>
  <span class="value_option"><data value="3">MASS_ERASE</data> <span class="comment">erase all flash memory banks</span> </span>
</details>
</li>

<li class="field"> <dfn>Reserved3</dfn> <span class="width">2</span>'@<span class="offset">3</span> </li>
<li class="field"> <dfn>SmartWrite</dfn> <span class="width">1</span>'@<span class="offset">5</span>
  <span class="comment"><q cite="(Ti User Guide slau208q, 7.4.1, p362)">If this bit is set, the program time is shortened. The programming quality has to be checked by marginal read modes.</q></span>
</li>

<li class="field"> <dfn>Write_BlockWrite</dfn> <span class="width">2</span>'@<span class="offset">6</span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">Reserved</data> </span>
  <span class="value_option"><data value="1">BYTE_OR_WORD_WRITE</data> </span>
  <span class="value_option"><data value="2">LONG_WORD_WRITE</data> </span>
  <span class="value_option"><data value="3">LONG_WORD_BLOCK_WRITE</data> </span>
</details>
</li>

<li class="field"> <dfn>ControlPassword</dfn> <span class="width">8</span>'@<span class="offset">8</span>
<span class="comment">Always reads as 096h (0x96 = 150). Must be written as 0A5h (0xA5 = 165) or a PUC is generated.</span>
</li>

</ul>
</details>
</li>

<li class="register" id="FlashMemoryModule.Control2">
Name: <dfn>Control2</dfn>. Width: <span class="width">16</span>.
This one is not in the slau208q User Guide.
It controls the Flash clock. But there is nothing on the clock in the guide.
<details>
<summary>
Fields.
</summary>

<ul>
<li class="field"> <dfn>DivideClock</dfn> <span class="width">6</span>'@<span class="offset">0</span>
<span class="comment"><q cite="msp430g2553.h header">Divide Flash clock by 1 to 64 using these (FN0-FN5) bits as:
32\*FN5 + 16\*FN4 + 8\*FN3 + 4\*FN2 + 2\*FN1 + FN0 + 1.</q>
So, it is just the field + 1.
</span>
</li>

<li class="field"> <dfn>ClockSelect</dfn> <span class="width">2</span>'@<span class="offset">6</span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">ACLK</data> </span>
  <span class="value_option"><data value="1">MCLK</data> </span>
  <span class="value_option"><data value="2">SMCLK</data> </span>
  <span class="value_option"><data value="3">SMCLK</data> </span>
</details>
</li>

<li class="field"> <dfn>ControlPassword</dfn> <span class="width">8</span>'@<span class="offset">8</span>
<span class="comment">Always reads as 096h (0x96 = 150). Must be written as 0A5h (0xA5 = 165) or a PUC is generated.</span>
</li>

</ul>
</details>
</li>

<li class="register" id="FlashMemoryModule.Control3">
Name: <dfn>Control3</dfn>. Width: <span class="width">16</span>.
<details>
<summary>
Fields.
</summary>

<ul>
<li class="field"> <dfn>Busy</dfn> <span class="width">1</span>'@<span class="offset">0</span> </li>
<li class="field"> <dfn>FlashKeyViolationFlag</dfn> <span class="width">1</span>'@<span class="offset">1</span> </li>
<li class="field"> <dfn>FlashAccessViolationFlag</dfn> <span class="width">1</span>'@<span class="offset">2</span> </li>
<li class="field"> <dfn>WaitFlag</dfn> <span class="width">1</span>'@<span class="offset">3</span>
  <span class="comment">for segment write</span>
</li>

<li class="field"> <dfn>LockBit</dfn> <span class="width">1</span>'@<span class="offset">4</span>
  <span class="comment">locked = read only</span>
</li>
<li class="field"> <dfn>FlashEmergencyExit</dfn> <span class="width">1</span>'@<span class="offset">5</span> </li>
<li class="field"> <dfn>LockBitSegmentA</dfn> <span class="width">1</span>'@<span class="offset">6</span>
  <span class="comment">locked = read only</span>
</li>
<li class="field"> <dfn>Fail</dfn> <span class="width">1</span>'@<span class="offset">7</span>
  <span class="comment">last program or erase failed</span>
</li>

<li class="field"> <dfn>ControlPassword</dfn> <span class="width">8</span>'@<span class="offset">8</span>
<span class="comment">Always reads as 096h (0x96 = 150). Must be written as 0A5h (0xA5 = 165) or a PUC is generated.</span>
</li>

</ul>
</details>
</li>

</ul>
</details>
</div>


The 4-registers version:

<div class="device_template" id="FlashMemoryModule4">
Device template name: <dfn class="cpp_name">FlashMemoryModule4</dfn>

<details>
<summary>
Registers.
</summary>

<ul>
<li class="register" id="FlashMemoryModule4.Control1">
Name: <dfn>Control1</dfn>. Width: <span class="width">16</span>.
<details>
<summary>
Fields.
</summary>

<ul>
<li class="field"> <dfn>Reserved0</dfn> <span class="width">1</span>'@<span class="offset">0</span> </li>
<li class="field"> <dfn>Erase_MassErase</dfn> <span class="width">2</span>'@<span class="offset">1</span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">NO_ERASE</data> </span>
  <span class="value_option"><data value="1">SEGMENT_ERASE</data> </span>
  <span class="value_option"><data value="2">BANK_ERASE</data> <span class="comment">erase one bank</span> </span>
  <span class="value_option"><data value="3">MASS_ERASE</data> <span class="comment">erase all flash memory banks</span> </span>
</details>
</li>

<li class="field"> <dfn>Reserved3</dfn> <span class="width">2</span>'@<span class="offset">3</span> </li>
<li class="field"> <dfn>SmartWrite</dfn> <span class="width">1</span>'@<span class="offset">5</span>
  <span class="comment"><q cite="(Ti User Guide slau208q, 7.4.1, p362)">If this bit is set, the program time is shortened. The programming quality has to be checked by marginal read modes.</q></span>
</li>

<li class="field"> <dfn>Write_BlockWrite</dfn> <span class="width">2</span>'@<span class="offset">6</span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">Reserved</data> </span>
  <span class="value_option"><data value="1">BYTE_OR_WORD_WRITE</data> </span>
  <span class="value_option"><data value="2">LONG_WORD_WRITE</data> </span>
  <span class="value_option"><data value="3">LONG_WORD_BLOCK_WRITE</data> </span>
</details>
</li>

<li class="field"> <dfn>ControlPassword</dfn> <span class="width">8</span>'@<span class="offset">8</span>
<span class="comment">Always reads as 096h (0x96 = 150). Must be written as 0A5h (0xA5 = 165) or a PUC is generated.</span>
</li>

</ul>
</details>
</li>

<li class="register" id="FlashMemoryModule4.Control2">
Name: <dfn>Control2</dfn>. Width: <span class="width">16</span>.
This one is not in the slau208q User Guide.
It controls the Flash clock. But there is nothing on the clock in the guide.
<details>
<summary>
Fields.
</summary>

<ul>
<li class="field"> <dfn>DivideClock</dfn> <span class="width">6</span>'@<span class="offset">0</span>
<span class="comment"><q cite="msp430g2553.h header">Divide Flash clock by 1 to 64 using these (FN0-FN5) bits as:
32\*FN5 + 16\*FN4 + 8\*FN3 + 4\*FN2 + 2\*FN1 + FN0 + 1.</q>
So, it is just the field + 1.
</span>
</li>

<li class="field"> <dfn>ClockSelect</dfn> <span class="width">2</span>'@<span class="offset">6</span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">ACLK</data> </span>
  <span class="value_option"><data value="1">MCLK</data> </span>
  <span class="value_option"><data value="2">SMCLK</data> </span>
  <span class="value_option"><data value="3">SMCLK</data> </span>
</details>
</li>

<li class="field"> <dfn>ControlPassword</dfn> <span class="width">8</span>'@<span class="offset">8</span>
<span class="comment">Always reads as 096h (0x96 = 150). Must be written as 0A5h (0xA5 = 165) or a PUC is generated.</span>
</li>

</ul>
</details>
</li>

<li class="register" id="FlashMemoryModule4.Control3">
Name: <dfn>Control3</dfn>. Width: <span class="width">16</span>.
<details>
<summary>
Fields.
</summary>

<ul>
<li class="field"> <dfn>Busy</dfn> <span class="width">1</span>'@<span class="offset">0</span> </li>
<li class="field"> <dfn>FlashKeyViolationFlag</dfn> <span class="width">1</span>'@<span class="offset">1</span> </li>
<li class="field"> <dfn>FlashAccessViolationFlag</dfn> <span class="width">1</span>'@<span class="offset">2</span> </li>
<li class="field"> <dfn>WaitFlag</dfn> <span class="width">1</span>'@<span class="offset">3</span>
  <span class="comment">for segment write</span>
</li>

<li class="field"> <dfn>LockBit</dfn> <span class="width">1</span>'@<span class="offset">4</span>
  <span class="comment">locked = read only</span>
</li>
<li class="field"> <dfn>FlashEmergencyExit</dfn> <span class="width">1</span>'@<span class="offset">5</span> </li>
<li class="field"> <dfn>LockBitSegmentA</dfn> <span class="width">1</span>'@<span class="offset">6</span>
  <span class="comment">locked = read only</span>
</li>
<li class="field"> <dfn>Fail</dfn> <span class="width">1</span>'@<span class="offset">7</span>
  <span class="comment">last program or erase failed</span>
</li>

<li class="field"> <dfn>ControlPassword</dfn> <span class="width">8</span>'@<span class="offset">8</span>
<span class="comment">Always reads as 096h (0x96 = 150). Must be written as 0A5h (0xA5 = 165) or a PUC is generated.</span>
</li>

</ul>
</details>
</li>

<li class="register" id="FlashMemoryModule4.Control4">
Name: <dfn>Control4</dfn>. Width: <span class="width">16</span>.
<details>
<summary>
Fields.
</summary>

<ul>
<li class="field"> <dfn>VoltageChangedDuringProgramError</dfn> <span class="width">1</span>'@<span class="offset">0</span>
  <span class="comment">
  <q cite="Ti User Guide slau208q, 7.4.3, p364">
  This bit is set by hardware and can only be cleared by software.
  If DVCC changed significantly during programming,
  this bit is set to indicate an invalid result.
  </q>
  </span>
</li>
<li class="field"> <dfn>Reserved1</dfn> <span class="width">3</span>'@<span class="offset">1</span> </li>
<li class="field"> <dfn>EnableMarginal0ReadMode</dfn> <span class="width">1</span>'@<span class="offset">4</span> </li>
<li class="field"> <dfn>EnableMarginal1ReadMode</dfn> <span class="width">1</span>'@<span class="offset">5</span> </li>
<li class="field"> <dfn>Reserved6</dfn> <span class="width">1</span>'@<span class="offset">6</span> </li>

<li class="field"> <dfn>LockInformationMemory</dfn> <span class="width">1</span>'@<span class="offset">7</span>
  <span class="comment">
  <q cite="Ti User Guide slau208q, 7.4.3, p364">
  If set, the information memory cannot be erased in segment erase mode and cannot be written to.
  </q>
  </span>
</li>

<li class="field"> <dfn>ControlPassword</dfn> <span class="width">8</span>'@<span class="offset">8</span>
<span class="comment">Always reads as 096h (0x96 = 150). Must be written as 0A5h (0xA5 = 165) or a PUC is generated.</span>
</li>

</ul>
</details>
</li>

</ul>
</details>
</div>


USCI A: UART (Asynchronous) and SPI (Synchronous) modes.
The configuration in `msp430g2553.h` and the User Guide slau208q:

<div class="device_template" id="USCI_A">
Device template name: <dfn class="cpp_name">USCI_A</dfn>

<details>
<summary>
Registers.
</summary>

<ul>
<li class="register" id="USCI_A.Control0">
Name: <dfn>Control0</dfn>. Width: <span class="width">8</span>.
<span class="comment">The User Guide says the first bit is:
0 = Asynchronous mode, 1 = Synchronous.
The header says it is UART and SPI.
Depending on the bit, the other bit fields set the behavior in these modes.
In Async Mode (bit = 0, UART), these are Async mode parameters, etc.
</span>
<details>
<summary>
Fields.
</summary>

<ul>

<li class="field"> <dfn>SyncMode</dfn> <span class="width">1</span>'@<span class="offset">0</span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">UART_MODE</data> </span>
  <span class="value_option"><data value="1">SPI_MODE</data> </span>
</details>
</li>

<li class="field"> <dfn>USCIMode</dfn> <span class="width">2</span>'@<span class="offset">1</span>
<span class="comment">
  Selects async mode when SyncMode = 0.
  And sync mode when SyncMode = 1.
</span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">UART_MODE_or_3pin_SPI</data> </span>
  <span class="value_option"><data value="1">IleLine_Multiprocessor_or_4pin_SPI_UCxSTE_active_HIGH</data> </span>
  <span class="value_option"><data value="2">AddressBit_Multiprocessor_or_4pin_SPI_UCxSTE_active_LOW</data> </span>
  <span class="value_option"><data value="3">AutomaticBaudRate_or_I2C_MODE</data> </span>
</details>
</li>

<li class="field"> <dfn>StopBits_or_MasterMode</dfn> <span class="width">1</span>'@<span class="offset">3</span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">ONE_STOP_BIT_or_SLAVE_MODE</data> </span>
  <span class="value_option"><data value="1">TWO_STOP_BITS_or_MASTER_MODE</data> </span>
</details>
</li>

<li class="field"> <dfn>CharacterLength</dfn> <span class="width">1</span>'@<span class="offset">4</span>
<span class="comment">
  <code>UC7BIT</code>
  Selects 7-bit or 8-bit character length.
</span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">CHAR_8bit</data> </span>
  <span class="value_option"><data value="1">CHAR_7bit</data> </span>
</details>
</li>

<li class="field"> <dfn>MSBFirst</dfn> <span class="width">1</span>'@<span class="offset">5</span>
<span class="comment">
  <code>UCMSB</code>
  Controls the direction of the receive and transmit shift register.
</span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">LSB_FIRST</data> </span>
  <span class="value_option"><data value="1">MSB_FIRST</data> </span>
</details>
</li>

<li class="field"> <dfn>Parity_or_Polarity</dfn> <span class="width">1</span>'@<span class="offset">6</span>
<span class="comment">
  <code>UCPAR</code> in Async (UART, Sync = 0)
  selects parity or is not used when parity is disabled by the last bitfield Parity <code>UCPEN</code>.
  <code>UCCKPL</code> in Sync (SPI, Sync = 1)
  selects clock polarity.
</span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">OddParity_or_InactiveStateIsLow</data> </span>
  <span class="value_option"><data value="1">EvenParity_or_InactiveStateIsHigh</data> </span>
</details>
</li>

<li class="field"> <dfn>ParityEnable_or_ClockPhaseSelect</dfn> <span class="width">1</span>'@<span class="offset">7</span>
<span class="comment">
  <code>UCPEN</code> in Async (UART, Sync = 0) enables parity: 
  parity bit is generated (UCAxTXD) and expected (UCAxRXD).
  In address-bit multiprocessor mode, the address bit is included in the parity calculation.
  <code>UCCKPL</code> in Sync (SPI, Sync = 1) selects clock phase:
  data is changed of the first UCLK edge and captured on the following edge,
  or is captured on the first edge and changed on the following.
</span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">ParityDisable_or_DataChangeCapture</data> </span>
  <span class="value_option"><data value="1">ParityEnable_or_DataCaptureChange</data> </span>
</details>
</li>

</ul>
</details>
</li>

<li class="register" id="USCI_A.Control1">
Name: <dfn>Control1</dfn>. Width: <span class="width">8</span>.
<span class="comment">
According to Sync mode,
0 = Asynchronous mode (UART) and 1 = Synchronous (SPI),
only few bit fields are used in Sync (SPI) mode.
So, when there is no `_or_` in a bitfield name,
the bitfield is used only in Async (UART) mode
or it has the same meaning in both modes.
</span>
<details>
<summary>
Fields.
</summary>

<ul>
<li class="field"> <dfn>SoftwareReset</dfn> <span class="width">1</span>'@<span class="offset">0</span>
  <span class="comment">Valid in both modes.
  When enabled (=1), the USCI logic is held in reset state.
  When disabled (=0), USCI released for operation.
  </span>
</li>

<li class="field"> <dfn>TransmitBreak</dfn> <span class="width">1</span>'@<span class="offset">1</span>
  <span class="comment">
  <code>UCTXBRK</code>
  Transmits a break with the next write to the transmit buffer.
  In UART mode with automatic baud-rate detection,
  <code>055h</code> (<code>0x55</code>) must be written into
  <code>UCAxTXBUF</code> to generate the required break/synch fields.
  Otherwise, <code>0x0</code> must be written into the transmit buffer.
  </span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">NEXT_FRAME_IS_NOT_A_BREAK</data> </span>
  <span class="value_option"><data value="1">NEXT_FRAME_IS_A_BREAK</data>
  <span class="comment">Next frame transmitted is a break or a break/synch.</span>
  </span>
</details>
</li>

<li class="field"> <dfn>TransmitAddress</dfn> <span class="width">1</span>'@<span class="offset">2</span>
  <span class="comment">
  <code>UCTXADDR</code>
  Transmits to be transmitted is marked as address,
  depending on the selected multiprocessor mode.
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">NEXT_FRAME_IS_DATA</data> </span>
  <span class="value_option"><data value="1">NEXT_FRAME_IS_ADDRESS</data> </span>
</details>
</li>

<li class="field"> <dfn>Dormant</dfn> <span class="width">1</span>'@<span class="offset">3</span>
  <span class="comment">
  <code>UCDORM</code>
  Puts USCI into sleep mode.
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">NOT_DORMANT</data>
  <span class="comment">All received characters set the flag <code>UCRXIFG</code>.</span>
  </span>
  <span class="value_option"><data value="1">DORMANT</data>
  <span class="comment">Only characters that are preceded by an idle-line or with address bit set <code>UCRXIFG</code>.
  In UART mode with automatic baud-rate detection, only the combination of a break and synch fields sets <code>UCRXIFG</code>.
  </span>
  </span>
</details>
</li>

<li class="field"> <dfn>RXBreakCharInterruptEnable</dfn> <span class="width">1</span>'@<span class="offset">4</span>
  <span class="comment">
  <code>UCBRKIE</code>
  Receive break character interrupt enable - set or no <code>UCRXIFG</code>.
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">RX_BREAK_DONOT_SET_FLAG</data>
  <span class="comment">Received break characters do not set the flag <code>UCRXIFG</code>.</span>
  </span>
  <span class="value_option"><data value="1">RX_BREAK_SET_FLAG</data>
  <span class="comment">Received break characters set the flag <code>UCRXIFG</code>.</span>
  </span>
</details>
</li>

<li class="field"> <dfn>RXErroneousCharInterruptEnable</dfn> <span class="width">1</span>'@<span class="offset">5</span>
  <span class="comment">
  <code>UCRXEIE</code>
  Receive erroneous-character interrupt enable - set or no <code>UCRXIFG</code>.
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">ERRCHARS_REJECTED_N_UCRXIFG_NOT_SET</data> </span>
  <span class="value_option"><data value="1">ERRCHARS_RECEIVED_N_UCRXIFG_SET</data> </span>
</details>
</li>

<li class="field"> <dfn>ClockSource</dfn> <span class="width">2</span>'@<span class="offset">6</span>
  <span class="comment">
  <code>UCSSELx</code>
  USCI clock source select. Select the <code>BRCLK</code> source clock.
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">UCAxCLK</data> <span class="comment">External USCI clock</span> </span>
  <span class="value_option"><data value="1">ACLK</data> </span>
  <span class="value_option"><data value="2">SMCLK</data> </span>
  <span class="value_option"><data value="3">SMCLK</data> </span>
</details>
</li>

</ul>
</details>
</li>

<li class="register" id="USCI_A.BaudRate0">
Name: <dfn>BaudRate0</dfn>. Width: <span class="width">8</span>.
<span class="comment">
<code>UCAxBR0</code>
Low byte of clock prescaler setting of the baud-rate generator.
The 16-bit value of <code>BaudRate0 + BaudRate1 \* 256</code>
forms the prescaler value <code>UCBRx</code>.
<code>f_bitclock = f_brclk / UCBRx</code>
<code>UCBRx = 0 => f_bitclock = f_brclk</code>
</span>
</li>

<li class="register" id="USCI_A.BaudRate1">
Name: <dfn>BaudRate1</dfn>. Width: <span class="width">8</span>.
<span class="comment">
<code>UCAxBR1</code>
High byte of clock prescaler setting of the baud-rate generator.
The 16-bit value of <code>BaudRate0 + BaudRate1 \* 256</code>
forms the prescaler value <code>UCBRx</code>.
<code>f_bitclock = f_brclk / UCBRx</code>
<code>UCBRx = 0 => f_bitclock = f_brclk</code>
</span>
</li>

<li class="register" id="USCI_A.ModulationControl">
Name: <dfn>ModulationControl</dfn>. Width: <span class="width">8</span>.
<span class="comment">
<code>UCAxMCTL</code>
Valid only in UART (Async) mode.
</span>
<details>
  <summary>Bit fields in UART (Async) mode.</summary>

<ul>
<li class="field"> <dfn>OversamplingEnable</dfn> <span class="width">1</span>'@<span class="offset">0</span>
  <span class="comment"> <code>UCOS16</code> </span>
</li>

<li class="field"> <dfn>SecondModulationStageSelect</dfn> <span class="width">3</span>'@<span class="offset">1</span>
  <span class="comment">
  <code>UCBRSx</code>
  These bits determine the modulation pattern for BITCLK. Table 36-2 in the slau208q User Guide.
  </span>
</li>

<li class="field"> <dfn>FirstModulationStageSelect</dfn> <span class="width">4</span>'@<span class="offset">4</span>
  <span class="comment">
  <code>UCBRFx</code>
  These bits determine the modulation pattern for BITCLK16 when Oversampling is enabled (UCOS16).
  Ignored when Oversampling is disabled.
  Table 36-2 in the slau208q User Guide.
  </span>
</li>

</ul>
</details>
</li>

<li class="register" id="USCI_A.Status">
Name: <dfn>Status</dfn>. Width: <span class="width">8</span>.
<span class="comment">
<code>UCAxSTAT</code>
Most bits are valid only for UART (Async).
The others are common to UART (Async) and SPI (SYNC) modes.
</span>
<details>
  <summary>Bit fields.</summary>

<ul>
<li class="field"> <dfn>Busy</dfn> <span class="width">1</span>'@<span class="offset">0</span>
  <span class="comment">
  <code>UCBUSY</code> Common to both modes.
  Indicated if a transmit or receive operation is in progress.
  </span>
</li>

<li class="field"> <dfn>AddrIdle</dfn> <span class="width">1</span>'@<span class="offset">1</span>
  <span class="comment">
  <code>UCADDR/UCIDLE</code> Only UART.
  <code>UCADDR</code>: address received in address-bit multiprocessor mode.
  Cleared when <code>UCAxRXBUF</code> is read.
  <code>UCIDLE</code>: idle line detected in idle-line multiprocessor mode.
  Cleared when <code>UCAxRXBUF</code> is read.
  </span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">DATA_or_NOIDLE</data> </span>
  <span class="value_option"><data value="1">ADDRESS_OR_IDLE_DETECTED</data> </span>
</details>
</li>

<li class="field"> <dfn>ErrorFlag</dfn> <span class="width">1</span>'@<span class="offset">2</span>
  <span class="comment">
  <code>UCRXERR</code> Only UART.
  Indicated a character was received with error(s).
  When <code>UCRXERR = 1</code>, one or more error flags, <code>UCFE UCPE UCOE</code>, is also set.
  Cleared when <code>UCAxRXBUF</code> is read.
  </span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">NO_ERRORS</data> </span>
  <span class="value_option"><data value="1">ERROR_DETECTED</data> </span>
</details>
</li>

<li class="field"> <dfn>BreakDetectedFlag</dfn> <span class="width">1</span>'@<span class="offset">3</span>
  <span class="comment">
  <code>UCBRK</code> Only UART.
  Cleared when <code>UCAxRXBUF</code> is read.
  </span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">NO_BREAK_CONDITIONS</data> </span>
  <span class="value_option"><data value="1">BREAK_CONDITION_OCCURED</data> </span>
</details>
</li>

<li class="field"> <dfn>ErrorFlagParity</dfn> <span class="width">1</span>'@<span class="offset">4</span>
  <span class="comment">
  <code>UCPE</code> Only UART.
  Cleared when <code>UCAxRXBUF</code> is read.
  </span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">NO_PARITY_ERROR</data> </span>
  <span class="value_option"><data value="1">WITH_PARITY_ERROR</data> </span>
</details>
</li>

<li class="field"> <dfn>ErrorFlagOverrun</dfn> <span class="width">1</span>'@<span class="offset">5</span>
  <span class="comment">
  <code>UCOE</code> Both modes.
  The bit is set when a character is transfered into <code>UCAxRXBUF</code> before the previous character was read.
  Cleared when <code>UCAxRXBUF</code> is read, and must not be cleared by software.
  Otherwise, it does not function correctly.
  </span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">NO_OVERRUN_ERROR</data> </span>
  <span class="value_option"><data value="1">OVERRUN_OCCURED</data> </span>
</details>
</li>

<li class="field"> <dfn>ErrorFlagFraming</dfn> <span class="width">1</span>'@<span class="offset">6</span>
  <span class="comment">
  <code>UCFE</code> Both modes.
  Framing error flag.
  Cleared when <code>UCAxRXBUF</code> is read.
  </span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">NO_FRAMING_ERROR</data> </span>
  <span class="value_option"><data value="1">WITH_LOW_STOP_BIT</data> </span>
</details>
</li>

<li class="field"> <dfn>ListenEnable</dfn> <span class="width">1</span>'@<span class="offset">7</span>
  <span class="comment">
  <code>UCLISTEN</code> Both modes.
  Selects loopback mode.
  When enabled (=1) the <code>UCAxTXD</code> is internally fed back to the receiver.
  </span>
</li>

</ul>
</details>
</li>

<li class="register" id="USCI_A.RXBuffer">
Name: <dfn>RXBuffer</dfn>. Width: <span class="width">8</span>.
<span class="comment">
<code>UCAxRXBUF</code>
The receive-data buffer is user-accessible and contains the last received character from the receive shift register.
Reading <code>UCAxRXBUF</code> resets the receive-error bits,
the Address/Idle <code>UCADDR/UCIDLE</code> bit, and the <code>UCRXIFG</code> (which I do not find in the header).
In 7-bit data mode, <code>UCAxRXBUF</code> is LSB justified and the MSB is always reset.
</span>
</li>

<li class="register" id="USCI_A.TXBuffer">
Name: <dfn>TXBuffer</dfn>. Width: <span class="width">8</span>.
<span class="comment">
<code>UCAxTXBUF</code>
The transmit-data buffer is user-accessible and contains the data waiting to be moved into the transmit shift register
and transmitted on <code>UCAxTXD</code>.
Writing to <code>UCAxTXBUF</code> clears <code>UCTXIFG</code> (which I do not find in the header).
The MSB of <code>UCAxTXBUF</code> is not used for 7-bit data and is reset.
</span>
</li>

<li class="register" id="USCI_A.AutoBaudRateControl">
Name: <dfn>AutoBaudRateControl</dfn>. Width: <span class="width">8</span>.
<span class="comment">
<code>UCAxABCTL</code> Only UART mode.
For some reason, the header calls it "LIN Control".
</span>
<details>
  <summary>Bit fields.</summary>
<ul>

<li class="field"> <dfn>Enable</dfn> <span class="width">1</span>'@<span class="offset">0</span>
  <span class="comment">
  <code>UCABDEN</code>
  Automatic baud-rate detect enable.
  When enabled (=1) the length of break and sync field is measured and baud-rate settings are changed accordingly.
  </span>
</li>

<li class="field"> <dfn>Reserved1</dfn> <span class="width">1</span>'@<span class="offset">1</span> </li>

<li class="field"> <dfn>TimeoutBreak</dfn> <span class="width">1</span>'@<span class="offset">2</span>
  <span class="comment">
  <code>UCBTOE</code>
  Break time out error.
  When set (=1), the length of break field exceeded 22 bit times.
  </span>
</li>

<li class="field"> <dfn>TimeoutSynch</dfn> <span class="width">1</span>'@<span class="offset">3</span>
  <span class="comment">
  <code>UCSTOE</code>
  Synch field time out error.
  When set (=1), the length of synch field exceeded measurable time.
  </span>
</li>

<li class="field"> <dfn>BreakSynchDelimiterLength</dfn> <span class="width">2</span>'@<span class="offset">4</span>
  <span class="comment">
  <code>UCDELIMx</code>
  Break and synch delimiter length.
  </span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">DELIM_1_BIT</data> </span>
  <span class="value_option"><data value="1">DELIM_2_BIT</data> </span>
  <span class="value_option"><data value="2">DELIM_3_BIT</data> </span>
  <span class="value_option"><data value="3">DELIM_4_BIT</data> </span>
</details>
</li>

<li class="field"> <dfn>Reserved6</dfn> <span class="width">2</span>'@<span class="offset">6</span> </li>

</ul>
</details>
</li>

<li class="register" id="USCI_A.IrDATransmitControl">
Name: <dfn>IrDATransmitControl</dfn>. Width: <span class="width">8</span>.
<span class="comment">
<code>UCAxIRTCTL</code> Only UART mode.
<code>USCI_Ax</code> IrDA Transmit Control Register.
</span>
<details>
  <summary>Bit fields.</summary>
<ul>

<li class="field"> <dfn>EncoderDecoderEnable</dfn> <span class="width">1</span>'@<span class="offset">0</span> </li>
<li class="field"> <dfn>TransmitPulseClockSelect</dfn> <span class="width">1</span>'@<span class="offset">1</span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">BRCLK</data> </span>
  <span class="value_option"><data value="1">BITCLK</data>
  <span class="comment">When Oversampling is ON (UCOS16 = 1). Otherwise, BRCLK.</span>
  </span>
</details>
</li>

<li class="field"> <dfn>TransmitLength</dfn> <span class="width">6</span>'@<span class="offset">2</span>
  <span class="comment">
  <code>UCIRTXPLx</code>
  The minimum pulse length for receive is given by:
  <code>t_pulse = (UCIRTXPLx + 1) / (2 \* f_irtxclk)</code>
  </span>
</li>

</ul>
</details>
</li>

<li class="register" id="USCI_A.IrDAReceiveControl">
Name: <dfn>IrDAReceiveControl</dfn>. Width: <span class="width">8</span>.
<span class="comment">
<code>UCAxIRRCTL</code> Only UART mode.
<code>USCI_Ax</code> IrDA Receive Control Register.
</span>
<details>
  <summary>Bit fields.</summary>
<ul>

<li class="field"> <dfn>FilterEnable</dfn> <span class="width">1</span>'@<span class="offset">0</span> </li>
<li class="field"> <dfn>Polarity</dfn> <span class="width">1</span>'@<span class="offset">1</span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">HIGH_PULSE_ON_LIGHT</data> </span>
  <span class="value_option"><data value="1">LOW_PULSE_ON_LIGHT</data> </span>
</details>
</li>

<li class="field"> <dfn>FilterLength</dfn> <span class="width">6</span>'@<span class="offset">2</span>
  <span class="comment">
  <code>UCIRRXFLx</code>
  The minimum pulse length for receive is given by:
  <code>t_min = (UCIRRXFLx + 4) / (2 \* f_brclk)</code>
  </span>
</li>

</ul>
</details>
</li>

</ul>
</details>
</div>

USCI B: SPI and I2C modes.
The configuration in `msp430g2553.h` and the User Guide slau208q:

<div class="device_template" id="USCI_B">
Device template name: <dfn class="cpp_name">USCI_B</dfn>

<details>
<summary>
Registers.
</summary>

<ul>
<li class="register" id="USCI_B.Control0">
Name: <dfn>Control0</dfn>. Width: <span class="width">8</span>.
<span class="comment">
<code>UCB0CTL0</code>
</span>
<details>
<summary>
Fields.
</summary>

<ul>

<li class="field"> <dfn>SyncMode</dfn> <span class="width">1</span>'@<span class="offset">0</span>
<span class="comment">
<code>UCSYNC</code>
Not sure what this one means here.
The <code>msp430g2553.h</code> header says it is Async=UART and Sync=SPI,
i.e. the same as for USCI A. Which cannot be true.
One should SPI and another one is I2C.
I guess, <code>SyncMode=ASYNCHRONOUS</code> and <code>USCIMode=I2C</code> selects I2C?
</span>

<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">ASYNCHRONOUS</data> </span>
  <span class="value_option"><data value="1">SYNCHRONOUS</data> </span>
</details>
</li>

<li class="field"> <dfn>USCIMode</dfn> <span class="width">2</span>'@<span class="offset">1</span>
<span class="comment">
<code>UCMODE0</code> and <code>UCMODE1</code>
</span>

<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">SYNC_3pin_SPI</data> </span>
  <span class="value_option"><data value="1">SYNC_4pin_SPI_UCxSTE_active_HIGH</data>
  <span class="comment">Slave enabled when <code>UCxSTE = 1</code>.</span>
  </span>
  <span class="value_option"><data value="2">SYNC_4pin_SPI_UCxSTE_active_LOW</data>
  <span class="comment">Slave enabled when <code>UCxSTE = 0</code>.</span>
  </span>
  <span class="value_option"><data value="3">I2C_MODE</data> </span>
</details>
</li>

<li class="field"> <dfn>MasterMode</dfn> <span class="width">1</span>'@<span class="offset">3</span>
<span class="comment">
<code>UCMST</code>
In I2C mode: when a master loses arbitration in a multi-master environment (<code>UCMM = 1</code>),
the <code>UCMST</code> bit is automatically cleared and the module acts as slave.
</span>

<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">SLAVE_MODE</data> </span>
  <span class="value_option"><data value="1">MASTER_MODE</data> </span>
</details>
</li>

<li class="field"> <dfn>CharacterLength</dfn> <span class="width">1</span>'@<span class="offset">4</span>
<span class="comment">
<code>UC7BIT</code>
In SPI mode: selects 7-bit or 8-bit character length.
In I2C: reserved, alwasy reads as 0.
</span>

<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">CHAR_8bit</data> </span>
  <span class="value_option"><data value="1">CHAR_7bit</data> </span>
</details>
</li>

<li class="field"> <dfn>MSBFirst_or_MultiMasterSelect</dfn> <span class="width">1</span>'@<span class="offset">5</span>
<span class="comment">
In SPI mode: <code>UCMSB</code>, controls the direction of the receive and transmit shift register.
In I2C mode: <code>UCMM</code>, selects single-master or multi-master environment.
</span>

<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">LSB_FIRST_or_SINGLE_MASTER</data>
  <span class="comment">I2C single-master: there is no other master in the system. The address compare unit is disabled.</span>
  </span>
  <span class="value_option"><data value="1">MSB_FIRST_or_MULTI_MASTER</data> </span>
</details>
</li>

<li class="field"> <dfn>Polarity_or_SlaveAddressingMode</dfn> <span class="width">1</span>'@<span class="offset">6</span>
<span class="comment">
In SPI (Sync) mode: <code>UCCKPL</code> selects clock polarity.
In I2C mode: address slave with 7-bit or 10-bit address.
</span>

<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">InactiveStateIsLow_or_Address7bit</data> </span>
  <span class="value_option"><data value="1">InactiveStateIsHigh_or_Address10bit</data> </span>
</details>
</li>

<li class="field"> <dfn>ClockPhaseSelect_or_OwnAddressingMode</dfn> <span class="width">1</span>'@<span class="offset">7</span>
<span class="comment">
In SPI (Sync) mode: <code>UCCKPL</code> selects clock phase:
data is changed of the first UCLK edge and captured on the following edge,
or is captured on the first edge and changed on the following.
In I2C mode: <code>UCA10</code> selects own address is 7-bit or 10-bit address.
</span>

<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">DataChangeCapture_or_OwnAddress7bit</data> </span>
  <span class="value_option"><data value="1">DataCaptureChange_or_OwnAddress10bit</data> </span>
</details>
</li>

</ul>
</details>
</li>

<li class="register" id="USCI_B.Control1">
Name: <dfn>Control1</dfn>. Width: <span class="width">8</span>.
<span class="comment">
<code>UCBxCTL1</code>
</span>
<details>
<summary>
Fields.
</summary>

<ul>
<li class="field"> <dfn>SoftwareReset</dfn> <span class="width">1</span>'@<span class="offset">0</span>
  <span class="comment">Valid in both modes.
  When enabled (=1), the USCI logic is held in reset state.
  When disabled (=0), USCI released for operation.
  </span>
</li>

<li class="field"> <dfn>TransmitStartCondition</dfn> <span class="width">1</span>'@<span class="offset">1</span>
<span class="comment">
  <code>UCTXSTT</code> Only I2C mode.
  Transmit START condition in master mode, ignored in slave mode.
  In master receiver mode, a repeated START condition is preceded by a NACK.
  <code>UCTXSTT</code> is automatically cleared after START comdition
  and address information is transmitted.
</span>

<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">DO_NOT_GENERATE_START_CONDITION</data> </span>
  <span class="value_option"><data value="1">GENERATE_START_CONDITION</data> </span>
</details>
</li>

<li class="field"> <dfn>TransmitStopCondition</dfn> <span class="width">1</span>'@<span class="offset">2</span>
<span class="comment">
  <code>UCTXSTP</code> Only in I2C mode.
  Transmit STOP condition in master mode. Ignored in slave mode.
  In master receiver mode, the STOP condition is preceded by a NACK.
  <code>UCTXSTP</code> is automatically cleared after STOP is generated.
</span>

<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">NO_STOP_GENERATED</data> </span>
  <span class="value_option"><data value="1">GENERATE_STOP</data> </span>
</details>
</li>

<li class="field"> <dfn>TransmitNACK</dfn> <span class="width">1</span>'@<span class="offset">3</span>
<span class="comment">
  <code>UCTXNACK</code> Only I2C mode.
  Transmit a NACK. <code>UCTXNACK</code> is automatically cleared after a NACK is transmitted.
</span>

<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">ACKNOWLEDGE_NORMALLY</data> </span>
  <span class="value_option"><data value="1">GENERATE_NACK</data> </span>
</details>
</li>

<li class="field"> <dfn>TransmitterOrReceiver</dfn> <span class="width">1</span>'@<span class="offset">4</span>
<span class="comment">
  <code>UCTR</code> Only in I2C.
</span>

<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">RECEIVER</data> </span>
  <span class="value_option"><data value="1">TRANSMITTER</data> </span>
</details>
</li>

<li class="field"> <dfn>Reserved</dfn> <span class="width">1</span>'@<span class="offset">5</span>
<span class="comment">
  Always reads 0.
</span>
</li>

<li class="field"> <dfn>ClockSource</dfn> <span class="width">2</span>'@<span class="offset">6</span>
<span class="comment">
  <code>UCSSELx</code> Valid in both SPI and I2C modes.
  USCI clock source select.
  In SPI mode: select the <code>BRCLK</code> source clock in master mode,
  <code>UCxCLK</code> is always used in slave mode.
  In I2C mode: select <code>BRCLK</code> source clock.
</span>

<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">Reserved_or_UCLKI</data> </span>
  <span class="value_option"><data value="1">ACLK</data> </span>
  <span class="value_option"><data value="2">SMCLK</data> </span>
  <span class="value_option"><data value="3">SMCLK</data> </span>
</details>
</li>

</ul>
</details>
</li>

<li class="register" id="USCI_B.BaudRate0">
Name: <dfn>BaudRate0</dfn>. Width: <span class="width">8</span>.
<span class="comment">
<code>UCBxBR0</code>
Low byte of clock prescaler setting of the baud-rate generator.
The 16-bit value of <code>BaudRate0 + BaudRate1 \* 256</code>
forms the prescaler value <code>UCBRx</code>.
<code>f_bitclock = f_brclk / UCBRx</code>
<code>UCBRx = 0 => f_bitclock = f_brclk</code>
</span>
</li>

<li class="register" id="USCI_B.BaudRate1">
Name: <dfn>BaudRate1</dfn>. Width: <span class="width">8</span>.
<span class="comment">
<code>UCBxBR1</code>
High byte of clock prescaler setting of the baud-rate generator.
The 16-bit value of <code>BaudRate0 + BaudRate1 \* 256</code>
forms the prescaler value <code>UCBRx</code>.
<code>f_bitclock = f_brclk / UCBRx</code>
<code>UCBRx = 0 => f_bitclock = f_brclk</code>
</span>
</li>

<li class="register" id="USCI_B.I2CInterruptEnable">
Name: <dfn>I2CInterruptEnable</dfn>. Width: <span class="width">8</span>.
<span class="comment">
<code>UCBxIE</code> or <code>UCBxI2CIE</code> in the <code>msp430g2552.h</code> header.
</span>
<details>
  <summary>Bit fields in I2C mode.</summary>

<ul>
<li class="field"> <dfn>ReceiveIE</dfn> <span class="width">1</span>'@<span class="offset">0</span> <span class="comment"> <code>UCRXIE</code> </span> </li>
<li class="field"> <dfn>TransmitIE</dfn> <span class="width">1</span>'@<span class="offset">1</span> <span class="comment"> <code>UCTXIE</code> </span> </li>
<li class="field"> <dfn>StartConditionIE</dfn> <span class="width">1</span>'@<span class="offset">2</span> <span class="comment"> <code>UCSTTIE</code> </span> </li>
<li class="field"> <dfn>StopConditionIE</dfn> <span class="width">1</span>'@<span class="offset">3</span> <span class="comment"> <code>UCSTPIE</code> </span> </li>
<li class="field"> <dfn>ArbitrationLostIE</dfn> <span class="width">1</span>'@<span class="offset">4</span> <span class="comment"> <code>UCALIE</code> </span> </li>
<li class="field"> <dfn>NotAcknowledgeIE</dfn> <span class="width">1</span>'@<span class="offset">5</span> <span class="comment"> <code>UCNACKIE</code> </span> </li>
<li class="field"> <dfn>Reserved</dfn> <span class="width">2</span>'@<span class="offset">6</span> <span class="comment"> <code>UCNACKIE</code> </span> </li>
</ul>
</details>
</li>

<li class="register" id="USCI_B.Status">
Name: <dfn>Status</dfn>. Width: <span class="width">8</span>.
<span class="comment">
<code>UCBxSTAT</code>
Can be modified only when <code>UCSWRST = 1</code>.
Most bits are valid only for I2C.
The others are common to I2C and SPI modes.
</span>
<details>
  <summary>Bit fields.</summary>

<ul>
<li class="field"> <dfn>Busy</dfn> <span class="width">1</span>'@<span class="offset">0</span>
  <span class="comment">
  <code>UCBUSY</code> only SPI.
  Indicated if a transmit or receive operation is in progress.
  </span>
</li>

<li class="field"> <dfn>Reserved1</dfn> <span class="width">3</span>'@<span class="offset">1</span>
</li>

<li class="field"> <dfn>I2CBusBusy</dfn> <span class="width">1</span>'@<span class="offset">4</span>
  <span class="comment">
  <code>UCBBUSY</code> only in I2C.
  </span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">BUS_INACTIVE</data> </span>
  <span class="value_option"><data value="1">BUS_BUSY</data> </span>
</details>
</li>

<li class="field"> <dfn>ErrorFlagOverrun_or_GeneralCallAddressReceived</dfn> <span class="width">1</span>'@<span class="offset">5</span>
<span class="comment">
  <code>UCOE</code> overrun flag in SPI.
  The bit is set when a character is transfered into <code>UCxRXBUF</code> before the previous character was read.
  Cleared when <code>UCxRXBUF</code> is read, and must not be cleared by software.
  Otherwise, it does not function correctly.
  <code>UCGC</code> general call address received in I2C.
  It is cleared automatically when a START condition is received.
</span>

<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">NO_OVERRUN_ERROR_or_NO_GENERAL_CALL_ADDRESS_RECEIVED</data> </span>
  <span class="value_option"><data value="1">OVERRUN_OCCURED_or_GENERAL_CALL_ADDRESS_RECEIVED</data> </span>
</details>
</li>

<li class="field"> <dfn>ErrorFlagFraming_or_SCLIsLow</dfn> <span class="width">1</span>'@<span class="offset">6</span>
<span class="comment">
  <code>UCFE</code> framing error flag in SPI.
  This bit indicates a bus conflict in 4-wire master mode.
  <code>UCFE</code> is not used in 3-wire master or any slave mode.
  <code>UCSCLLOW</code> in I2C: SCL is held low.
</span>

<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">NO_ERROR_or_SCL_NOT_LOW</data> </span>
  <span class="value_option"><data value="1">BUS_CONFLICT_OCCURED_or_SCL_IS_LOW</data> </span>
</details>
</li>

<li class="field"> <dfn>ListenEnable</dfn> <span class="width">1</span>'@<span class="offset">7</span>
  <span class="comment">
  <code>UCLISTEN</code> only SPI mode.
  Selects loopback mode.
  When enabled (=1) the transmitter output is internally fed back to the receiver.
  </span>
</li>

</ul>
</details>
</li>

<li class="register" id="USCI_B.RXBuffer">
Name: <dfn>RXBuffer</dfn>. Width: <span class="width">8</span>.
<span class="comment">
<code>UCBxRXBUF</code>
The receive-data buffer is user-accessible and contains the last received character from the receive shift register.
Reading <code>UCBxRXBUF</code> resets the receive-error bits,
the Address/Idle <code>UCADDR/UCIDLE</code> bit, and the <code>UCRXIFG</code> (which I do not find in the header).
In 7-bit data mode, <code>UCBxRXBUF</code> is LSB justified and the MSB is always reset.
</span>
</li>

<li class="register" id="USCI_B.TXBuffer">
Name: <dfn>TXBuffer</dfn>. Width: <span class="width">8</span>.
<span class="comment">
<code>UCBxTXBUF</code>
The transmit-data buffer is user-accessible and contains the data waiting to be moved into the transmit shift register
and transmitted on <code>UCAxTXD</code>.
Writing to <code>UCBxTXBUF</code> clears <code>UCTXIFG</code> (which I do not find in the header).
The MSB of <code>UCBxTXBUF</code> is not used for 7-bit data and is reset.
</span>
</li>

<li class="register" id="USCI_B.I2COwnAddress">
Name: <dfn>I2COwnAddress</dfn>. Width: <span class="width">16</span>.
<span class="comment">
<code>UCBxI2COA</code>
Can be modified only when <code>UCSWRST = 1</code>.
</span>

<details>
<summary>Bit fields.</summary>
<ul>

<li class="field"> <dfn>OwnAddress</dfn> <span class="width">10</span>'@<span class="offset">0</span>
<span class="comment">
  <code>I2COAx</code> I2C own address, the local address of
  the <code>USCI_Bx</code> I2C controller.
  The address is right justified.
  In 7-bit addressing mode, bit 6 is the MSB and bits 9-7 are ignored.
  In 10-bit addressing mode, bit 9 is MSB.
</span>
</li>

<li class="field"> <dfn>Reserved</dfn> <span class="width">5</span>'@<span class="offset">10</span> </li>

<li class="field"> <dfn>GeneralCallResponseEnable</dfn> <span class="width">1</span>'@<span class="offset">15</span>
<span class="comment">
  <code>UCGCEN</code>
</span>

<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">DO_NOT_RESPOND_TO_GENERAL_CALL</data> </span>
  <span class="value_option"><data value="1">RESPOND_TO_GENERAL_CALL</data> </span>
</details>
</li>

</ul>
</details>
</li>

<li class="register" id="USCI_B.I2CSlaveAddress">
Name: <dfn>I2CSlaveAddress</dfn>. Width: <span class="width">16</span>.
<span class="comment">
<code>UCBxI2CSA</code>
Can be modified only when <code>UCSWRST = 1</code>.
</span>

<details>
<summary>Bit fields.</summary>
<ul>

<li class="field"> <dfn>SlaveAddress</dfn> <span class="width">10</span>'@<span class="offset">0</span>
<span class="comment">
  <code>I2CSAx</code> I2C slave address of the external device to be addressed by
  the <code>USCI_Bx</code> module. It is only used in master mode.
  The address is right justified.
  In 7-bit slave addressing mode, bit 6 is the MSB and bits 9-7 are ignored.
  In 10-bit slave addressing mode, bit 9 is MSB.
</span>
</li>

<li class="field"> <dfn>Reserved</dfn> <span class="width">6</span>'@<span class="offset">10</span> </li>

</ul>
</details>
</li>

</ul>
</details>
</div>


Watchdog timer.
The configuration in `msp430g2553.h` and the User Guide slau208q:

<div class="device_template" id="WatchdogTimer">
Device template name: <dfn class="cpp_name">WatchdogTimer</dfn>

<details>
<summary>
Registers.
</summary>

<ul>

<li class="register" id="WatchdogTimer.Control">
Name: <dfn>Control</dfn>. Width: <span class="width">16</span>.
<span class="comment">
<code>WDTCTL</code>
The lower byte is the control, the higher byte is the password:
<code>WDTPW = 0x5A</code> (always reads as <code>0x69</code>).
</span>

<details>
<summary>Bit fields.</summary>
<ul>

<li class="field"> <dfn>TimerIntervalSelect</dfn> <span class="width">3</span>'@<span class="offset">0</span>
<span class="comment">
  <code>WDTIS</code> Select the watchdog timer interval
  to set <code>WDTIFG</code> flag or generate a PUC.
  The User Guide says it is a 3-bit field.
  But the <code></code> header says the third bit is <code>WDTSSEL</code> - source clock select?
  But the User Guide says the clock select field <code>WDTSSEL</code> is at 5-6 bits.
  I go with the Guide.
</span>

<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">CLOCK_DIV_2e31</data> <span class="comment">18h:12m:16s at 32.768 kHz</span> </span>
  <span class="value_option"><data value="1">CLOCK_DIV_2e27</data> <span class="comment">01h:08m:16s at 32.768 kHz</span> </span>
  <span class="value_option"><data value="2">CLOCK_DIV_2e23</data> <span class="comment">00h:04m:16s at 32.768 kHz</span> </span>
  <span class="value_option"><data value="3">CLOCK_DIV_2e19</data> <span class="comment">00h:00m:16s at 32.768 kHz</span> </span>
  <span class="value_option"><data value="4">CLOCK_DIV_2e15</data> <span class="comment">1s at 32.768 kHz</span> </span>
  <span class="value_option"><data value="5">CLOCK_DIV_2e13</data> <span class="comment">250ms at 32.768 kHz</span> </span>
  <span class="value_option"><data value="6">CLOCK_DIV_2e9</data> <span class="comment">15.625ms at 32.768 kHz</span> </span>
  <span class="value_option"><data value="7">CLOCK_DIV_2e6</data> <span class="comment">1.95ms at 32.768 kHz</span> </span>
</details>
</li>

<li class="field"> <dfn>CounterClear</dfn> <span class="width">1</span>'@<span class="offset">3</span>
<span class="comment"> <code>WDTCNTCL</code> clears the count value to 0.  </span>
</li>

<li class="field"> <dfn>ModeSelect</dfn> <span class="width">1</span>'@<span class="offset">4</span>
<span class="comment"> <code>WDTTMSEL</code> </span>

<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">WATCHDOG</data> </span>
  <span class="value_option"><data value="1">INTERVAL_TIMER</data> </span>
</details>
</li>

<li class="field"> <dfn>ClockSource</dfn> <span class="width">2</span>'@<span class="offset">5</span>
<span class="comment"> <code>WDTSSEL</code> </span>

<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">SMCLK</data> </span>
  <span class="value_option"><data value="1">ACLK</data> </span>
  <span class="value_option"><data value="2">VLOCLK</data> </span>
  <span class="value_option"><data value="3">X_CLK</data>
  <span class="comment"> <code>VLOCLK</code> in devices that do not use <code>X_CLK</code> </span>
  </span>
</details>
</li>

<li class="field"> <dfn>TimerHold</dfn> <span class="width">1</span>'@<span class="offset">7</span>
<span class="comment"> <code>WDTHOLD</code> Stops the watchdog timer.
<code>WDTHOLD = 1</code> when WDT is not in use conserves power.
</span>

<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">NOT_STOPPED</data> </span>
  <span class="value_option"><data value="1">STOPPED</data> </span>
</details>
</li>

</ul>
</details>
</li>

</ul>
</details>
</div>

Calibration data:

<div class="device_template" id="CalibrationData">
Device template name: <dfn class="cpp_name">CalibrationData</dfn>

<details>
<summary>
Registers.
</summary>

<ul>

<li class="register" id="CalibrationData.DCO_16MHZ">
Name: <dfn>DCO_16MHZ</dfn>. Width: <span class="width">8</span>.
<span class="comment"> <code>CALDCO_16MHZ</code> DCOCTL Calibration Data for 16MHz.  </span>
</li>

<li class="register" id="CalibrationData.BC1_16MHZ">
Name: <dfn>BC1_16MHZ</dfn>. Width: <span class="width">8</span>.
<span class="comment"> <code>CALBC1_16MHZ</code> BCSCTL1 Calibration Data for 16MHz.  </span>
</li>

<li class="register" id="CalibrationData.DCO_12MHZ">
Name: <dfn>DCO_12MHZ</dfn>. Width: <span class="width">8</span>.
<span class="comment"> <code>CALDCO_12MHZ</code> DCOCTL Calibration Data for 12MHz.  </span>
</li>

<li class="register" id="CalibrationData.BC1_12MHZ">
Name: <dfn>BC1_12MHZ</dfn>. Width: <span class="width">8</span>.
<span class="comment"> <code>CALBC1_12MHZ</code> BCSCTL1 Calibration Data for 12MHz.  </span>
</li>

<li class="register" id="CalibrationData.DCO_8MHZ">
Name: <dfn>DCO_8MHZ</dfn>. Width: <span class="width">8</span>.
<span class="comment"> <code>CALDCO_8MHZ</code> DCOCTL Calibration Data for 8MHz.  </span>
</li>

<li class="register" id="CalibrationData.BC1_8MHZ">
Name: <dfn>BC1_8MHZ</dfn>. Width: <span class="width">8</span>.
<span class="comment"> <code>CALBC1_8MHZ</code> BCSCTL1 Calibration Data for 8MHz.  </span>
</li>

<li class="register" id="CalibrationData.DCO_1MHZ">
Name: <dfn>DCO_1MHZ</dfn>. Width: <span class="width">8</span>.
<span class="comment"> <code>CALDCO_1MHZ</code> DCOCTL Calibration Data for 1MHz.  </span>
</li>

<li class="register" id="CalibrationData.BC1_1MHZ">
Name: <dfn>BC1_1MHZ</dfn>. Width: <span class="width">8</span>.
<span class="comment"> <code>CALBC1_1MHZ</code> BCSCTL1 Calibration Data for 1MHz.  </span>
</li>

<li class="register" id="CalibrationData.TLVChecksum">
Name: <dfn>TLVChecksum</dfn>. Width: <span class="width">16</span>.
<span class="comment"> <code>TLV_CHECKSUM</code> TLV check sum  </span>
</li>

<li class="register" id="CalibrationData.TLVDCO30Tag">
Name: <dfn>TLVDCO30Tag</dfn>. Width: <span class="width">8</span>.
<span class="comment"> <code>TLV_DCO_30_TAG</code> TLV TAG_DCO30 tag  </span>
</li>

<li class="register" id="CalibrationData.TLVDCO30Len">
Name: <dfn>TLVDCO30Len</dfn>. Width: <span class="width">8</span>.
<span class="comment"> <code>TLV_DCO_30_LEN</code> TLV TAG_DCO30 len  </span>
</li>

<li class="register" id="CalibrationData.TLVADC10Tag">
Name: <dfn>TLVADC10Tag</dfn>. Width: <span class="width">8</span>.
<span class="comment"> <code>TLV_ADC10_1_TAG</code> TLV ADC10_1 tag  </span>
</li>

<li class="register" id="CalibrationData.TLVADC10Len">
Name: <dfn>TLVADC10Len</dfn>. Width: <span class="width">8</span>.
<span class="comment"> <code>TLV_ADC10_1_LEN</code> TLV ADC10_1 len  </span>
</li>

</ul>
</details>
</div>

TODO:

* `USCI_B` seems to have the same registers and bit field mapping as A.
  + I also do not find the `UCAxIE` (Interrup Enable) and `UCAxIFG` (Interrupt Flag) registers
  in the `msp430g2553.h` header.
  + Neither regs `UCBxIE` and `UCBxIFG` and `UCBxIV`?
  + Where is `UCSWRST`?
* Calibration data -- check what is DCO30. (ADC10 is clear.)
* Flash Memory: Check the datasheet for MSP430G2553.
The registers are not entirely clear.
Where is `Control2` in the slau208q User Guide?
Where is `Control4` in the `msp430g2553.h` header?
Is it an optional control register?
Where is the flash memory access violation interrupt bit?
  + Also, what's up with the password byte?
    Do you have to write it every time when you write some modifications
    in the control registers? If you read one thing `0x96` but have to
    write another one `0xA5`, then my current bit-field manipulation code won't work.
    Probably, in this case, the `Register` must have a default value and mask.

