# Welcome to My Bc

## Task
Create a simple expression evaluator in pure C. It evaluates basic math expressions including basic alphanumeric characters.

## Description
The problem was solved by using a couple of steps:

1. Input validation   
The input is being validated thoroughly by several criteria at a time. 
Your input __*MUST NOT*__ contain the following:
- Just one number as an expression. This doesn't make sense.
- An operator followed by a closing brace (`)`)
- An operator followed by a non-suiting next one (e.g. `+` followed by a `/`, `-` followed by a `*`, any mixing thereof OR vice-versa.)
- Text characters of ANY kind
2. Conversion  
When validated, the expression gets converted to [Reverse-Polish notation](https://en.wikipedia.org/wiki/Reverse_Polish_notation) as part of the assignment. Conversion happens using a simple stack in [main.c:47](./main.c#L47)
3. Evaluation  
Using the output from the above step, we simply evaluate the expression using a linear stack evaluation. This happens in [main.c:146](./main.c#L146)

## Installation
Just do the following:
```
make && ./my_bc "<EXPRESSION>"
```

> Note that if the expression provided is empty some tests will run, showing the evaluation at work in a user-friendly way.

Compiled using gcc 9.4.0, `\_\_STDC_VERSION__ --> 201710L` ([C version 17](https://en.wikipedia.org/wiki/C17_(C_standard_revision)))

## Usage
How does it work? - Check the [Description](#description) point. 
Instructions how to run are on the [Installation](#installation) point.

### The Core Team
- [Petar (Shannarra) Angelov](https://www.github.com/Shannarra)
- Mykyta Moisieiev
