#pragma once

#ifndef FDEFINES_H
#define FDEFINES_H

#define FMAX(x,y) (x > y ? x : y)
#define FMIN(x,y) (x > y ? y : x)

#define FCLAMP(value, min, max) (value <= min) ? min : (value >= max) ? max : value;

#endif
