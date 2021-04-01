# Content

This is a package that efficiently computes Shapley value and Banzhaf index of large weighted voting games.
Additionaly, it provides methods to compute the power indices of sums of weighted voting games and sums of unanimity games.

# Installation

The package requires libntl and libgmp.
If you do not have those and your system uses apt, you can install them with
```
sudo apt install libgmp-dev libntl-dev
```

Also additional R packages may be required. Those can be installed with
```
R -e "install.packages('Rcpp')"
```

After that, build the package by running.
```
make install
```

This installs the R package and builds executable binary file ./bin/voting-games.
Run ./bin/voting-games to see help on its usage.

# Examples

See the examples provided in the folder 'examples'

# References

This implementation is based on my master thesis, which can be found here:
<https://dspace.cvut.cz/bitstream/handle/10467/87786/F3-DP-2020-Kristan-Jan%20Matyas-Kristan-thesis-2020.pdf?sequence=-1&isAllowed=y>
