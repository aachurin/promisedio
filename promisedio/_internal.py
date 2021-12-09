# Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
# This file is part of promisedio

import warnings


def __sigtrap():
    # do nothing, just a trap
    pass


def __after_fork():
    warnings.warn("fork not supported and can lead to unpredictable results", RuntimeWarning)