% Ridiculous Digital Datasheet for MSP430G2xx and MSP430F2xx
% Alex Toldaiev
% February 2, 2026

# Introduction

Testing an old idea: single-source description of chips.

* Have a very readable and writable and parseable document:
  + Markdown and HTML generated with Pandoc.
* And generate C++ headers from the parseable version,
using very well fitting C++ constructions, so that the header-generation
is trivial and does not encode any actual C++ logic.

Use the Ti User Guide for MSP430F2xx and MSP430G2xx chips.
And the `msp430g2553.h` header for the MSP430G2553 model.

Let's try on the TimerA and its Capture/Compare blocks.

# TimerA

<details>
<summary class="device" id="TimerA">
<dfn>TimerA</dfn>
</summary>

    <details>
    <summary class="register" id="TimerA.Status">
    <dfn>Control</dfn>
    </summary>

        <summary class="field"> <dfn>taifg</dfn>   <data class="offset">0</data> <data class="width">1</data> </summary>
        <summary class="field"> <dfn>taie</dfn>    <data class="offset">1</data> <data class="width">1</data> </summary>
        <summary class="field"> <dfn>clear</dfn>   <data class="offset">2</data> <data class="width">1</data> </summary>
        <summary class="field"> <dfn>unused3</dfn> <data class="offset">3</data> <data class="width">1</data> </summary>

        <details class="field"> <summary> <dfn>mode_control</dfn> <data class="offset">4</data> <data class="width">2</data> </summary>
            <data class="value_option" value="0">MODE_STOP</data>
            <data class="value_option" value="1">MODE_UPTO</data>
            <data class="value_option" value="2">MODE_CONT</data>
            <data class="value_option" value="3">MODE_UPDN</data>
        </details>

        <details class="field"> <summary> <dfn>clock_divider</dfn> <data class="offset">6</data> <data class="width">2</data> </summary>
            <data class="value_option" value="0">DIVIDE_0</data>
            <data class="value_option" value="1">DIVIDE_2</data>
            <data class="value_option" value="2">DIVIDE_4</data>
            <data class="value_option" value="3">DIVIDE_8</data>
        </details>

        <details class="field"> <summary> <dfn>input_clock</dfn> <data class="offset">8</data> <data class="width">3</data> </summary>
            <data class="value_option" value="0">INPUT_CLOCK_TACLK</data>
            <data class="value_option" value="1">INPUT_CLOCK_ACLK</data>
            <data class="value_option" value="2">INPUT_CLOCK_SMCLK</data>
            <data class="value_option" value="3">INPUT_CLOCK_INCLK</data>
        </details>

        <summary class="field"> <dfn>unused11</dfn> <data class="offset">11</data> <data class="width">5</data> </summary>
    </details>

    <details>
    <summary class="device" id="TimerA.CaptureCompareBlock">
    <dfn>CaptureCompareBlock</dfn>
    </summary>

        <details>
        <summary class="register" id="TimerA.CaptureCompareBlock.Control">
        <dfn>CapComControl</dfn>
        </summary>
            <summary class="field"> <dfn>interrupt_flag</dfn>   <data class="offset">0</data> <data class="width">1</data> </summary>
            <summary class="field"> <dfn>capture_overflow</dfn> <data class="offset">1</data> <data class="width">1</data> </summary>
            <summary class="field"> <dfn>out_x</dfn>            <data class="offset">2</data> <data class="width">1</data> </summary>
            <summary class="field"> <dfn>capcom_input</dfn>     <data class="offset">3</data> <data class="width">1</data> </summary>

            <summary class="field"> <dfn>interrupt_enable</dfn>   <data class="offset">4</data> <data class="width">1</data> </summary>
            <summary class="field"> <dfn>output_mode</dfn>        <data class="offset">5</data> <data class="width">3</data> </summary>

            <summary class="field"> <dfn>capture_or_compare</dfn> <data class="offset">8</data> <data class="width">1</data> </summary>
            <summary class="field"> <dfn>dummy_0</dfn>            <data class="offset">9</data> <data class="width">1</data> </summary>
            <summary class="field"> <dfn>sync_capcom_input</dfn>  <data class="offset">10</data> <data class="width">1</data> </summary>
            <summary class="field"> <dfn>async_sync_capture</dfn> <data class="offset">11</data> <data class="width">1</data> </summary>

            <summary class="field"> <dfn>input_select</dfn>       <data class="offset">12</data> <data class="width">2</data> </summary>
            <summary class="field"> <dfn>capture_mode</dfn>       <data class="offset">14</data> <data class="width">2</data> </summary>
        </details>

        <dt class="register" id="TimerA.CaptureCompareBlock.Register">
        <dfn>CapComReg</dfn>
        </dt>
    </details>

</details>

