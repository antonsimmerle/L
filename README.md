# L

The L repository is a **UNIX file system** inspired tree structure automator.

With L it is possible to make boring structures look nice.

<table>
<tr>
<th>Input</th>
<th>Output</th>
</tr>
<tr>
<td>

```
Title
 Heading
 Heading
  Body
  Body
 Heading
 Heading
  Body
Title
 Heading
  Body
 Heading
```

</td>
<td>

```
├─ Title
│   ├─ Heading
│   ├─ Heading
│   │   ├─ Body
│   │   └─ Body
│   ├─ Heading
│   └─ Heading
│       └─ Body
└─ Title
    ├─ Heading
    │   └─ Body
    └─ Heading
```

</td>
</tr>
</table>

> To step higher into the hierarchy you continuously **add one space**.

L was first published 281223 by Anton Simmerle.

## Requirements

* GCC
* Standard C library (libc)

## Compatibility

L uses only standard C libraries such as 'stdio.h' and 'stdlib.h', so it should be compatible with UNIX and non UNIX systems.

## Limits

L uses non-exponential dynamic memory allocation for the amount of nodes as well as the depth of the nodes, but falls back to fixed sized arrays for the length of the text of the node (1024 characters).

## Compilation

```bash
gcc -o main main.c
```

## Usage

```bash
Usage: ./main <input.txt>
```
