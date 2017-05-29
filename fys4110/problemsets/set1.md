e--
header-includes: 
    \usepackage{bbm}
---

\renewcommand{\vec}[1]{\mathbf{#1}}
\newcommand{\la}{\langle}
\newcommand{\ra}{\rangle}
\newcommand{\ip}[2]{\la #1 | #2 \ra}
\newcommand{\ev}[3]{\la #1 | #2 | #3 \ra}
\newcommand{\bra}[1]{\la #1 |}
\newcommand{\ket}[1]{| #1 \ra}
\newcommand{\dd}[1]{\mathrm{d}#1\;}

# Problem Set Week 1

## 1.4 Position and Momentum Eigenstate
We have the relation

\begin{align*}
    \int \dd x |x\ra \la x| = \mathbbm{1} \Rightarrow \int \dd x \ip{x}{x'}\ip{x'}{p} 
    = \ip{x}{p}
\end{align*}
