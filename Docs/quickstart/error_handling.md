# Error Handling

Entix's main way to handle errors is {cpp:class}`Entix::Result`. This class are inspired by Rust's `Result`
and doing same things.

:::{doxygenclass} Entix::Result
:project: Entix Engine
:members:
:undoc-members:

:::

:::{doxygendefine} EX_TRY
:project: Entix Engine

:::

:::{doxygendefine} EX_LET_TRY
:project: Entix Engine

:::

## Unrecovarable errors

:::{note}
In Debug build {cpp:func}`Entix::Panic` also place debug breakpoint
:::

:::{doxygenfunction} Entix::Panic(std::source_location)
:project: Entix Engine

:::

:::{doxygenfunction} Entix::Panic(StringView, std::source_location)
:project: Entix Engine

:::

## Assertions

Assertions just checking provided expression and if it evaluated to `false` call the {cpp:func}`Entix::Panic`
with custom message.

:::{attention}
Assertion macro are enabled only in Debug build!
:::

:::{doxygendefine} EX_ASSERT
:project: Entix Engine

:::

:::{doxygendefine} EX_ASSERT_FMT
:project: Entix Engine

:::
