%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Package for my course note at SUPINFO
% Author: Alexandre JOSEPH
% Created: November 3, 2009
% Modified: November 3, 2009
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

\NeedsTeXFormat{LaTeX2e}
\ProvidesPackage{supinfo}

\LoadClassWithOptions{article}[12pt,a4paper]


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Loaded package
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

\usepackage{amsmath}
\usepackage{hyperref}
\usepackage{graphicx}
\usepackage{pifont}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Commons commands shortcut
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

\author{
Quentin\hfill\textsc{Amoudruz}\\
Thomas\hfill\textsc{Savineau}\\
Alexandre\hfill\textsc{Joseph}\\
Lucas\hfill\textsc{Courot}
}


\newcommand{\ditem}[1]{\item[#1] \hfill \\}

\renewcommand{\date}[2]{\gdef\@date{\emph{Created:} #1\\\emph{Modified:} #2}}
%\newcommand{\created}[1]{\emph{Created:} #1}
%\newcommand{\modified}[1]{\emph{Modified:} #1}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Common styles
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Remove paragraph indent
\setlength{\parindent}{0in}
% Set margin between paragraph
\setlength{\parskip}{1ex minus0.5ex}
% Disable title numbering
\setcounter{secnumdepth}{-1}


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Document header
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

\renewcommand{\maketitle}[0]{
    \begin{titlepage}
    \begin{center}
     
    % Upper part of the page 
    \includegraphics{supinfo-logo}\\[1cm]
    \textsc{\LARGE SUPINFO International University}\\[0.4cm]
    \textsc{\Large School year 2009 -- 2010}\\[1.5cm]
     
    % Title
    \hrule height 1pt\hfill\\[0.4cm]
    {\huge \bfseries \@title}\\[1cm]
    \hrule height 1pt\hfill\\[1.6cm]
    
    \begin{minipage}{2in}
    \large{\@author}
    \end{minipage}

    \vfill
    \begin{flushright}
    Last update: \@date
    \end{flushright}
     
    \end{center}
    \end{titlepage}
}
