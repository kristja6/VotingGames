#!/usr/bin/python3
import csv, os, subprocess
from termcolor import colored

command = "/home/maty/Dropbox/FEL/diplomka/package/VotingGames/bin/coal-game"
arguments_bz = [
    "--banzhaf --normal --naive"
    , "--banzhaf --normal --uno"
    , "--banzhaf --opt --new-only-convolution"
    ,"--banzhaf --opt --new-with-deconvolution"
    ,"--banzhaf --opt --new-dp"
    ]

arguments_sh = [
    "--shapley --opt --new-for-each"
    , "--shapley --opt --new-only-convolution"
    , "--shapley --opt --new-dp"
    , "--shapley --normal --new"
    , "--shapley --normal --uno"
    , "--shapley --normal --new-dp"
    ]

files = [
    "./inputs/voting/small.in"
    , "./inputs/voting/test1.in"
    , "./inputs/voting/seq6.in"
    , "./inputs/voting/seq20.in"
    , "./inputs/voting/with_zero.in"]


add = 0
error = False
#for arguments in [arguments_bz, arguments_sh]: 
for arguments in [arguments_bz]: 
  for input_file in files:
    cnt = add

    for argument in arguments:
      cmd = command + " " + argument + " < " + input_file + " > /tmp/" + str(cnt) + ".txt"
      os.system(cmd)
      cnt += 1

    cnt = add
    for argument in arguments:
      if os.system("diff /tmp/" + str(add) + ".txt /tmp/" + str(cnt) + ".txt") != 0:
        print(input_file + ": " + argument)
        error = True
      cnt += 1
    add += 10

  add = 100
if error: print(colored("Error!", "red"))
else: print(colored("OK", "green"))
