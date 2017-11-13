# mean
A LL Grammar Parser for C++11.  
### Quickstart
```
expr: term ( ('+' | '-') term )*
term: factor ( ('*' | '/') factor )*
factor: base [ '^' term ]
base: '(' expr ')' |  NUMBER
```

Will output:
```
|expr -- 100
  |factor -- 100
    |base -- 100
      |2 -- 26
    |^ -- 2
    |base -- 100
      |6 -- 26
  |[.] -- 30
    |- -- 2
    |base -- 100
      |1 -- 26
```
