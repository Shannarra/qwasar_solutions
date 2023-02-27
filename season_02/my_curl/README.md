# Welcome to My CURL

## Task
Create a dumbed-down replica of the [`linux curl command`](https://linux.die.net/man/1/curl) and prove it's usability by fetching the HTML from a given url as ARGV.

## Description
The problem requires exploration and basic understanding of low-level of network protocols and communication. This means that the product (ergo `my_curl`) will NOT support any of the flags supported by the linux command OR any HTTP verbs other than GET.

## Installation
Just do the following:
```sh
make && make clean && ./my_curl "<URL_HERE>"
```

Compiled using gcc 9.4.0, `__STDC_VERSION__ --> 201710L` ([C version 17](https://en.wikipedia.org/wiki/C17_(C_standard_revision)))

### Output piping
If you want to, you can pipe the output from the console into a file and use it to check the returned value of the HTTP request. Just do the following:

```sh
make && make clean && ./my_curl "<URL_HERE>" >> text.html
```

Example:
```
make re && make clean && ./my_curl http://www.columbia.edu/\~fdc/sample.html >> text.html
```

## Usage
How does it work? - Check the [Description](#description) point.
Instructions how to run are on the [Installation](#installation) point.

Tested URLS:
- `./my_curl http://www.columbia.edu/~fdc/sample.html`
- `./my_curl example.com`
- `./my_curl man7.org/linux/man-pages/man1/curl.1.html`
- `./my_curl http://httpbin.org/`
- `./my_curl www.w3.org`
- `./my_curl developer.mozilla.org`

## *!NOTE!*
A lot of URLs may not produce an output. This is due to either an empty response body, OR a status code that's different from `200 (OK)`.
Please note that due to the requirements for the task, `my_curl` supports ONLY HTTP.

## The Core Team
- [Petar (Shannarra) Angelov](https://www.github.com/Shannarra)
- Mykyta Moisieiev
