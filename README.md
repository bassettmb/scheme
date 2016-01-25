# scheme

A scheme-inspired lisp (name yet to be determined).

Design Goals:
  + Functional (with a pure bent)
    + tail call elimination required
    + curried by default
  + Easily embeddable
    + interpreter should hold no implicit global state
    + should be feasible to start multiple independent interpreters inside one process
    + should provide simple C ffi
    
Resources:
  + [Scheme R5RS Standard \[pdf\]](http://www.schemers.org/Documents/Standards/R5RS/r5rs.pdf)
  + [Making a fast curry \[pdf\]](http://community.haskell.org/~simonmar/papers/evalapplyjfp06.pdf)
  + [The Metacircular Evaluator](https://mitpress.mit.edu/sicp/full-text/sicp/book/node76.html)

  
