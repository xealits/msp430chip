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

First, let's try `this`{.foo} feature in Pandoc?

[The whole paragraph as a span?]{.foo}

And let's try it on lists?

* []{.foo}Here is a top item -- no way to attach the class to the item.
  - And a [sub-item]{.bar}.

* Another top item.

Yep, it's BS.

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
