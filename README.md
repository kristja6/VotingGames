# Content

This is a package that efficiently computes Shapley value and Banzhaf index of large weighted voting games.
Additionaly, it provides methods to compute the power indices of sums of weighted voting games and sums of unanimity games.

# Installation

The package requires libntl and libgmp.
If you do not have those and your system uses apt, you can install them with
```
sudo apt install libgmp-dev libntl-dev
```

After that, build the package by running.
```
make install
```

Installs the R package and builds executable binary file ./bin/coal-game.
Run ./bin/coal-game to see help on its usage.

# Examples

See the examples provided in the folder 'examples'
