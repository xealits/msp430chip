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

<div class="device" id="TimerA">
<summary>
Device template name: <dfn class="cpp_name">TimerA</dfn>

TimerA device info summary.
</summary>

<details>
<summary>
Registers and sub-devices.
</summary>

<ul>
<li>
<div class="register" id="TimerA.Control">
<summary>
Register type name: <dfn>Control</dfn>
</summary>
<details>
<summary>
Control register fields.
</summary>

<ul>
<li><div class="field"> <dfn>taifg</dfn>   <data class="offset">0</data> <data class="width">1</data> </div></li>
<li><div class="field"> <dfn>taie</dfn>    <data class="offset">1</data> <data class="width">1</data> </div></li>
<li><div class="field"> <dfn>clear</dfn>   <data class="offset">2</data> <data class="width">1</data> </div></li>
<li><div class="field"> <dfn>unused3</dfn> <data class="offset">3</data> <data class="width">1</data> </div></li>

<li>
<div class="field"><dfn>mode_control</dfn>
<data class="offset">4</data> <data class="width">2</data>
<details>
  <summary>Value options.</summary>
  <data class="value_option" value="0">MODE_STOP</data>
  <data class="value_option" value="1">MODE_UPTO</data>
  <data class="value_option" value="2">MODE_CONT</data>
  <data class="value_option" value="3">MODE_UPDN</data>
</details>
</div>
</li>

<li>
<div class="field"> <dfn>clock_divider</dfn>
<data class="offset">6</data> <data class="width">2</data>
<details>
  <summary>Value options.</summary>
  <data class="value_option" value="0">DIVIDE_0</data>
  <data class="value_option" value="1">DIVIDE_2</data>
  <data class="value_option" value="2">DIVIDE_4</data>
  <data class="value_option" value="3">DIVIDE_8</data>
</details>
</div>
</li>

<li>
<div class="field"> <dfn>input_clock</dfn>
<data class="offset">8</data> <data class="width">3</data>
<details>
  <summary>Value options.</summary>
  <data class="value_option" value="0">INPUT_CLOCK_TACLK</data>
  <data class="value_option" value="1">INPUT_CLOCK_ACLK</data>
  <data class="value_option" value="2">INPUT_CLOCK_SMCLK</data>
  <data class="value_option" value="3">INPUT_CLOCK_INCLK</data>
</details>
</div>
</li>

<li>
<div class="field"> <dfn>unused11</dfn>
<data class="offset">11</data> <data class="width">5</data>
</div>
</li>

</ul>
</details>
</div>
</li>

<li>
<div class="register" id="TimerA.TimerReg">
Register type name: <dfn>TimerReg</dfn>
</div>
</li>

<li>
<div class="device" id="TimerA.CaptureCompareBlock">
Device template name: <dfn>CaptureCompareBlock</dfn>.

TimerA can have 2-3 identical Capture/Compare blocks.
(The TimerA devices have only 2 blocks in MSP430 models
that have two TimerA devices, 0 and 1.)
<details>
<summary>
Registers and subdevices of a Capture/Compare Block.
</summary>

<ul>
<li>
<div class="register" id="TimerA.CaptureCompareBlock.Control">
Register type name: <dfn>CapComControl</dfn>
<details>
<summary>
Fields.
</summary>

<div class="field"> <dfn>interrupt_flag</dfn>   <data class="offset">0</data> <data class="width">1</data> </div>
<div class="field"> <dfn>capture_overflow</dfn> <data class="offset">1</data> <data class="width">1</data> </div>
<div class="field"> <dfn>out_x</dfn>            <data class="offset">2</data> <data class="width">1</data> </div>
<div class="field"> <dfn>capcom_input</dfn>     <data class="offset">3</data> <data class="width">1</data> </div>

<div class="field"> <dfn>interrupt_enable</dfn>   <data class="offset">4</data> <data class="width">1</data> </div>
<div class="field"> <dfn>output_mode</dfn>        <data class="offset">5</data> <data class="width">3</data> </div>

<div class="field"> <dfn>capture_or_compare</dfn> <data class="offset">8</data> <data class="width">1</data> </div>
<div class="field"> <dfn>dummy_0</dfn>            <data class="offset">9</data> <data class="width">1</data> </div>
<div class="field"> <dfn>sync_capcom_input</dfn>  <data class="offset">10</data> <data class="width">1</data> </div>
<div class="field"> <dfn>async_sync_capture</dfn> <data class="offset">11</data> <data class="width">1</data> </div>

<div class="field"> <dfn>input_select</dfn>       <data class="offset">12</data> <data class="width">2</data> </div>
<div class="field"> <dfn>capture_mode</dfn>       <data class="offset">14</data> <data class="width">2</data> </div>
</details>
</div>
</li>

<li>
<div class="register" id="TimerA.CaptureCompareBlock.Register">
Register type name: <dfn>CapComReg</dfn>.
</div>
</li>

</ul>

</details>

</div>
</li>

<li>
<div class="register" id="TimerA.InterruptVector">
Register type name: <dfn>InterruptVector</dfn>
<details>
<summary>Fields.</summary>

<div class="field"> <dfn>contents</dfn> <data class="offset">1</data> <data class="width">4</data> </div>
</details>
</div>
</li>

</ul>
</details>
</div>
