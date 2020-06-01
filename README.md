#### Cooperative Threading Library
This is one-evening exercise project of implementing very basic cooperative multithreading for C language using Linux x86_64 ABI. Repo includes simple scheduler, context switcher and usage example.

Build dependencies: `gcc`, `nasm`, `make`. Currently Valgrind, while analysing sample executable, produces errors which are pointing to `ctl_linuxx64.asm:54`; suppression file is included as I couldn't figure out error root cause.

License: MIT
Author: Jevgenijs Protopopovs