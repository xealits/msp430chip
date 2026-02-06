---
title: Ridiculous Digital Datasheet for MSP430G2xx and MSP430F2xx
author: Alex Toldaiev
date: February 2, 2026
---

# Introduction

Testing an old idea: single-source description of chips.

* Have a very readable and writable and parseable document:
  + Markdown and HTML generated with Pandoc.
* And generate C++ headers from the parseable version,
using very well fitting C++ constructions, so that the header-generation
is trivial and does not encode any actual C++ logic.
* It should also somehow maintain a good link between the generated doxygen docs and the original document. Add URLs in doxygen. But with respect to what? It needs some good citation to an HTML document.

Use the Ti User Guide for MSP430F2xx and MSP430G2xx chips.
And the `msp430g2553.h` header for the MSP430G2553 model.

Let's try on the TimerA and its Capture/Compare blocks.

# TimerA

Any text with notes, links, code, all normal markdown.
And also descriptions of devices.

<div class="device_template" id="TimerA">
Device template name: <dfn class="cpp_name">TimerA</dfn>

TimerA device info summary.

<details>
<summary>
Registers and sub-devices.
</summary>

<ul>
<li class="register" id="TimerA.Control">
Register type name: <dfn>Control</dfn>
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

<li class="register" id="TimerA.TimerReg">
Register type name: <dfn>TimerReg</dfn>
</li>

<li class="device_template" id="TimerA.CaptureCompareBlock">
Device template name: <dfn>CaptureCompareBlock</dfn>.

TimerA can have 2-3 identical Capture/Compare blocks.
(The TimerA devices have only 2 blocks in MSP430 models
that have two TimerA devices, 0 and 1.)
<details>
<summary>
Registers and subdevices of a Capture/Compare Block.
</summary>

<ul>
<li class="register" id="TimerA.CaptureCompareBlock.Control">
Register type name: <dfn>CapComControl</dfn>
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

<li class="register" id="TimerA.InterruptVector">
Register type name: <dfn>InterruptVector</dfn>
<details>
<summary>Fields.</summary>

<ul>
<li class="field"> <dfn>contents</dfn> <span class="offset">1</span> <span class="width">4</span> </li>
</ul>
</details>

</li>
</ul>
</details>
</div>

One more top-level device, an 8-bit IO port:

<div class="device_template" id="PortIO8bit">
Device template name: <dfn class="cpp_name">PortIO8bit</dfn>

IO port control.

<details>
<summary>
Registers.
</summary>

<ul>
<li class="register" id="PortIO8bit.p_in"> Name: <dfn>p_in</dfn>. Width: <span class="width">8</span>. </li>
<li class="register" id="PortIO8bit.p_out"> Name: <dfn>p_out</dfn>. Width: <span class="width">8</span>. </li>
<li class="register" id="PortIO8bit.p_dir"> Name: <dfn>p_dir</dfn>. Width: <span class="width">8</span>. </li>
<li class="register" id="PortIO8bit.p_sel"> Name: <dfn>p_sel</dfn>. Width: <span class="width">8</span>. </li>
<li class="register" id="PortIO8bit.p_sel2"> Name: <dfn>p_sel2</dfn>. Width: <span class="width">8</span>. </li>
<li class="register" id="PortIO8bit.p_en"> Name: <dfn>p_en</dfn>. Width: <span class="width">8</span>. </li>
</ul>
</details>
</div>

Same 8-bit IO port with interrupts control:

<div class="device_template" id="PortIO8bitI">
Device template name: <dfn class="cpp_name">PortIO8bitI</dfn>

IO port control with interrupts.

<details>
<summary>
Registers.
</summary>

<ul>
<li class="register" id="PortIO8bitI.p_in"> Name: <dfn>p_in</dfn>. Width: <span class="width">8</span>. </li>
<li class="register" id="PortIO8bitI.p_out"> Name: <dfn>p_out</dfn>. Width: <span class="width">8</span>. </li>
<li class="register" id="PortIO8bitI.p_dir"> Name: <dfn>p_dir</dfn>. Width: <span class="width">8</span>. </li>

<li class="register" id="PortIO8bitI.p_sel"> Name: <dfn>p_sel</dfn>. Width: <span class="width">8</span>. </li>
<li class="register" id="PortIO8bitI.p_sel2"> Name: <dfn>p_sel2</dfn>. Width: <span class="width">8</span>. </li>
<li class="register" id="PortIO8bitI.p_en"> Name: <dfn>p_en</dfn>. Width: <span class="width">8</span>. </li>

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
<li class="field"> <dfn>fetch</dfn> <span class="offset">0</span> <span class="width">1</span> </li>
<li class="field"> <dfn>block_one</dfn> <span class="offset">1</span> <span class="width">1</span> </li>
<li class="field"> <dfn>continuous_transfer</dfn> <span class="offset">2</span> <span class="width">1</span> </li>
<li class="field"> <dfn>two_block_mode</dfn> <span class="offset">3</span> <span class="width">1</span> </li>
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
<li class="field"> <dfn>fetch</dfn> <span class="offset">0</span> <span class="width">1</span> </li>
<li class="field"> <dfn>block_one</dfn> <span class="offset">1</span> <span class="width">1</span> </li>
<li class="field"> <dfn>continuous_transfer</dfn> <span class="offset">2</span> <span class="width">1</span> </li>
<li class="field"> <dfn>two_block_mode</dfn> <span class="offset">3</span> <span class="width">1</span> </li>
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
<li class="field"> <dfn>start_conversion</dfn> <span class="offset">0</span> <span class="width">1</span> </li>
<li class="field"> <dfn>enable_conversion</dfn> <span class="offset">1</span> <span class="width">1</span> </li>
<li class="field"> <dfn>interrupt_flag</dfn> <span class="offset">2</span> <span class="width">1</span> </li>
<li class="field"> <dfn>interrupt_enable</dfn> <span class="offset">3</span> <span class="width">1</span> </li>
<li class="field"> <dfn>on_enable</dfn> <span class="offset">4</span> <span class="width">1</span> </li>
<li class="field"> <dfn>ref_on</dfn> <span class="offset">5</span> <span class="width">1</span> </li>
<li class="field"> <dfn>ref_setting</dfn> <span class="offset">6</span> <span class="width">1</span>
0 = 1.5V, 1 = 2.5V
</li>
<li class="field"> <dfn>multiple_sample_conversion</dfn> <span class="offset">7</span> <span class="width">1</span> </li>

<li class="field"> <dfn>ref_burst_mode</dfn> <span class="offset">8</span> <span class="width">1</span> </li>
<li class="field"> <dfn>enable_output_of_ref</dfn> <span class="offset">9</span> <span class="width">1</span> </li>
<li class="field"> <dfn>sampling_rate</dfn> <span class="offset">10</span> <span class="width">1</span>
0 = 200ksps, 1 = 50ksps
</li>

<li class="field"> <dfn>sample_hold_select</dfn> <span class="offset">11</span> <span class="width">2</span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">SH_x4</data> <span class="comment">4 x ADC10 clocks</span></span>
  <span class="value_option"><data value="1">SH_x8</data> <span class="comment">8 x ADC10 clocks</span></span>
  <span class="value_option"><data value="2">SH_x16</data> <span class="comment">16 x ADC10 clocks</span></span>
  <span class="value_option"><data value="3">SH_x64</data> <span class="comment">64 x ADC10 clocks</span></span>
</details>
</li>

<li class="field"> <dfn>reference_select</dfn> <span class="offset">13</span> <span class="width">3</span>
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
<li class="field"> <dfn>busy</dfn> <span class="offset">0</span> <span class="width">1</span> </li>
<li class="field"> <dfn>conversion_sequence</dfn> <span class="offset">1</span> <span class="width">2</span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">SingleChannelSingleConversion</data> </span>
  <span class="value_option"><data value="1">SequenceOfChannels</data> </span>
  <span class="value_option"><data value="2">RepeatSingleChannel</data> </span>
  <span class="value_option"><data value="3">RepeatSequenceOfChannels</data> </span>
</details>
</li>

<li class="field"> <dfn>clock_source</dfn> <span class="offset">3</span> <span class="width">2</span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">ADC10OSC</data> </span>
  <span class="value_option"><data value="1">ACLK</data> </span>
  <span class="value_option"><data value="2">MCLK</data> </span>
  <span class="value_option"><data value="3">SMCLK</data> </span>
</details>
</li>

<li class="field"> <dfn>clock_divider</dfn> <span class="offset">5</span> <span class="width">3</span>
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

<li class="field"> <dfn>invert_sample_hold</dfn> <span class="offset">8</span> <span class="width">1</span> </li>
<li class="field"> <dfn>data_format</dfn> <span class="offset">9</span> <span class="width">1</span>
<span class="comment">0 = binary, 1 = 2's complement</span>
</li>
<li class="field"> <dfn>sample_hold_source</dfn> <span class="offset">10</span> <span class="width">2</span>
<details>
  <summary>Value options.</summary>
  <span class="value_option"><data value="0">SHS_ADC10OSC</data> </span>
  <span class="value_option"><data value="1">TA3_OUT1</data> </span>
  <span class="value_option"><data value="2">TA3_OUT0</data> </span>
  <span class="value_option"><data value="3">TA3_OUT2</data> </span>
</details>
</li>

<li class="field"> <dfn>input_channel</dfn> <span class="offset">12</span> <span class="width">4</span>
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



