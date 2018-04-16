% !Mode:: "TeX:UTF-8"
\NeedsTeXFormat{LaTeX2e}[1999/12/01]
\ProvidesClass{WHUBachelor}[2016/05/31 4.0 Wuhan University Thesis Template for Bachelor Degree]
\LoadClass[zihao=-4,a4paper,oneside]{ctexbook}
\setmainfont{Times New Roman}

\DeclareOption*{\PassOptionsToClass{\CurrentOption}{ctexbook}}
\ProcessOptions
\newif\ifprint\printfalse
\DeclareOption{forprint}{\printtrue}
\ProcessOptions


%------------------------ Page layout ----------------------------------
\RequirePackage{fancyhdr,hyperref}

\hypersetup{pdfencoding=auto}  %%% 邓东升修改
 \ifprint
 \hypersetup{hidelinks}
 \else
\hypersetup{citecolor=magenta,linkcolor=blue}
\fi
\RequirePackage[top=2.7truecm,bottom=2.2truecm,left=3truecm,right=3truecm,includefoot,xetex]{geometry}    % 页面设置%

\newtoks\StudentNumber       %学号
\newtoks\miji            %密级
\newtoks\Cmajor          %学科专业中文名
\newtoks\Emajor          %学科专业英文名
\newtoks\Edate           %英文日期
\newtoks\Etitle          %英文题目
\newtoks\Eauthor         %作者英文名
\newtoks\Esupervisor     %指导教师英文名
\newtoks\Csupervisor     %指导教师中文名
\newtoks\Eschoolname      %学院英文名
\newtoks\Cschoolname      %学院中文名



\pagestyle{fancy}
\fancyhf{}  %清除以前对页眉页脚的设置
\renewcommand{\headrulewidth}{0pt}
\fancyfoot[C]{-\,\thepage\,-}
% \lhead{实验名称：\title}
% \chead{姓名：\@author}
% \rhead{学号：\StudentNumber}


\fancypagestyle{plain}
{
\fancyhead{}                                    % clear all header fields
\renewcommand{\headrulewidth}{0pt}
\fancyfoot{}                                    % clear all footer fields
\fancyfoot[C]{-\,\thepage\,-}
}

\def\equationautorefname{式}%
\def\footnoteautorefname{脚注}%
\def\itemautorefname{项}%
\def\figureautorefname{图}%
\def\tableautorefname{表}%
\def\partautorefname{篇}%
\def\appendixautorefname{附录}%
\def\chapterautorefname{章}%
\def\sectionautorefname{节}%
\def\subsectionautorefname{小小节}%
\def\subsubsectionautorefname{subsubsection}%
\def\paragraphautorefname{段落}%
\def\subparagraphautorefname{子段落}%
\def\FancyVerbLineautorefname{行}%
\def\theoremautorefname{定理}%

%%=====封面===================== 
\def\maketitle{%
  \null
  \thispagestyle{empty}%
  %\vspace*{-2cm}
  \begin{center}\leavevmode
  %   {\heiti \zihao{5}%
  %   \begin{tabular}{p{9cm}lp{3cm}l}
  % & \raisebox{-0.5ex}[0pt]{\makebox[1.5cm][s]{学\hfill 号}} & {}\hfill\raisebox{-0.5ex}[0pt]{\the\StudentNumber}\hfill{} & \\ \cline{3-3}
  % & \raisebox{-0.5ex}[0pt]{\makebox[1.5cm][s]{密\hfill 级}} & {}\hfill\raisebox{-0.5ex}[0pt]{\the\miji}\hfill{}   & \\ \cline{3-3}
  %   \end{tabular}
  %   }
    \par
    \vspace*{2.1cm} %插入空白
    {\songti \zihao{-1} \makebox[9cm][s]{操作系统原理实验报告}}\\
    \vspace{1.6cm}
    {\heiti \zihao{2} \@title \par}%
%    \vspace{4cm}
     \vfill\vfill\vfill
    {\songti\zihao{-3}
    \begin{tabular}{cp{5cm}c}
      \raisebox{-3ex}[0pt]{\makebox[3.5cm][s]{院\hfill (系)\hfill 名\hfill 称：}} & {  {}\raisebox{-3ex}[0pt]{\the\Cschoolname}\hfill{}} & \\[3ex]
      \raisebox{-3ex}[0pt]{\makebox[3.5cm][s]{专\hfill 业\hfill 名\hfill 称：}} & { {}\raisebox{-3ex}[0pt]{\the\Cmajor}\hfill{}} & \\[3ex]
      \raisebox{-3ex}[0pt]{\makebox[3.5cm][s]{学\hfill 生\hfill 姓\hfill 名：}} & { {}\raisebox{-3ex}[0pt]{\@author}\hfill{}} & \\[3ex]
      \raisebox{-3ex}[0pt]{\makebox[3.5cm][s]{学\hfill 生\hfill 学\hfill 号：}} & { {}\raisebox{-3ex}[0pt]{\the\StudentNumber}\hfill{}} & \\[3ex]
      \raisebox{-3ex}[0pt]{\makebox[3.5cm][s]{指\hfill 导\hfill 教\hfill 师：}} & { {}\raisebox{-3ex}[0pt]{\the\Csupervisor}\hfill{}} & \\[3ex]
     \end{tabular}
    }
    \par
    \vspace{2.6cm}
    {
    {\songti \zihao{3} \@date \par}%
    }
    %\vspace*{-0.7cm}
  \end{center}%
%  \null
%   \cleardoublepage
  }

%------------------------ Abstract & Keywords -------------------------
\newcommand\cnabstractname{摘要} 
\newcommand\enabstractname{ABSTRACT}
\newcommand\cnkeywords[1]{ {\noindent\heiti\zihao{-4}\cnkeywordsname: }#1}
\newcommand\cnkeywordsname{关键词}
\newcommand\enkeywords[1]{ {\noindent\bfseries\zihao{-4}\enkeywordsname: }#1}
\newcommand\enkeywordsname{Key words}

\newenvironment{cnabstract}{
    \newpage
\chapter[\cnabstractname]{\ziju{2}{\cnabstractname}}   %%%终于解决了书签乱码的问题 2005-04-12.huangzh
    \songti\zihao{-4}
    \@afterheading}
    {\par}

\newenvironment{enabstract}{
    \newpage
    \chapter[\enabstractname]{\enabstractname}
    \songti\zihao{-4}
    \@afterheading}
    {\par}

%%------------------------常用宏包-----------------------------------
\RequirePackage{amsmath,amssymb}
\RequirePackage[amsmath,thmmarks]{ntheorem}% AMSLaTeX宏包.
\RequirePackage{graphicx}                 % 图形
\RequirePackage[dvipsnames]{color}             %支持彩色
\RequirePackage[dvipsnames,svgnames,table]{xcolor}             %支持彩色
%\RequirePackage{mathrsfs}   % 不同于\mathcal or \mathfrak 之类的英文花体字体
\RequirePackage{url}
\RequirePackage{enumerate}
\RequirePackage{booktabs}
% \RequirePackage{identfirst}
% \setlength {\ parindent }{2em}
% 段间距
\addtolength{\parskip}{4pt}

%%----------------------- Theorems -----------------------------------
\theoremstyle{plain}
\theoremheaderfont{\heiti}
\theorembodyfont{\songti} \theoremindent0em
\theorempreskip{0pt}
\theorempostskip{0pt}
\theoremnumbering{arabic}
%\theoremsymbol{} %定理结束时自动添加的标志
\newtheorem{theorem}{\hspace{2em}定理}[section]
\newtheorem{definition}{\hspace{2em}定义}[section]
\newtheorem{lemma}{\hspace{2em}引理}[section]
\newtheorem{corollary}{\hspace{2em}推论}[section]
\newtheorem{proposition}{\hspace{2em}性质}[section]
\newtheorem{example}{\hspace{2em}例}[section]
\newtheorem{remark}{\hspace{2em}注}[section]

\theoremstyle{nonumberplain}
\theoremheaderfont{\heiti}
\theorembodyfont{\normalfont \rm \songti}
\theoremindent0em \theoremseparator{\hspace{1em}}
\theoremsymbol{$\square$}
\newtheorem{proof}{\hspace{2em}证明}


\newcommand{\upcite}[1]{\textsuperscript{\cite{#1}}}  %自定义新命令\upcite, 使参考文献引用以上标出现

%--------------------------code------------------------------

\RequirePackage{listings}
% \usepackage[usenames,dvipsnames]{color}
 
\definecolor{DarkGreen}{rgb}{0.0,0.4,0.0}
 
\lstloadlanguages{c}
\definecolor{CPPLight}  {HTML} {686868}
\definecolor{CPPSteel}  {HTML} {888888}
\definecolor{CPPDark}   {HTML} {262626}
\definecolor{CPPBlue}   {HTML} {4172A3}
\definecolor{CPPGreen}  {HTML} {487818}
\definecolor{CPPBrown}  {HTML} {A07040}
\definecolor{CPPRed}    {HTML} {AD4D3A}
\definecolor{CPPViolet} {HTML} {7040A0}
\definecolor{CPPGray}  {HTML} {B8B8B8}
\lstset{
    columns=fixed,       
    numbers=left,                                        % 在左侧显示行号
    frame=shadowbox,                                          % 不显示背景边框
    backgroundcolor=\color[RGB]{245,245,244},            % 设定背景颜色
    keywordstyle=\color[RGB]{40,40,255},                 % 设定关键字颜色
    numberstyle=\footnotesize\color{darkgray},           % 设定行号格式
    commentstyle=\it\color[RGB]{0,96,96},                % 设置代码注释的格式
    stringstyle=\rmfamily\slshape\color[RGB]{128,0,0},   % 设置字符串格式
    showstringspaces=false,                              % 不显示字符串中的空格
    language=c,                                        % 设置语言
    morekeywords={alignas,continute,friend,register,true,alignof,decltype,goto,
    reinterpret_cast,try,asm,defult,if,return,typedef,auto,delete,inline,short,
    typeid,bool,do,int,signed,typename,break,double,long,sizeof,union,case,
    dynamic_cast,mutable,static,unsigned,catch,else,namespace,static_assert,using,
    char,enum,new,static_cast,virtual,char16_t,char32_t,explict,noexcept,struct,
    void,export,nullptr,switch,volatile,class,extern,operator,template,wchar_t,
    const,false,private,this,while,constexpr,float,protected,thread_local,
    const_cast,for,public,throw,std},
    emph={map,set,multimap,multiset,unordered_map,unordered_set,
    unordered_multiset,unordered_multimap,vector,string,list,deque,
    array,stack,forwared_list,iostream,memory,shared_ptr,unique_ptr,
    random,bitset,ostream,istream,cout,cin,endl,move,default_random_engine,
    uniform_int_distribution,iterator,algorithm,functional,bing,numeric,},
    emphstyle=\color{CPPViolet}, 
}




%%%%%%%-------------------------------------------------
%%%===  suppress extra inter-line spacing in \list environments
\makeatletter \setlength\partopsep{0pt}
\def\@listI{\leftmargin\leftmargini
            \parsep 0pt
            \topsep \parsep
            \itemsep \parsep}
\@listI
\def\@listii {\leftmargin\leftmarginii
              \labelwidth\leftmarginii
              \advance\labelwidth-\labelsep
              \parsep    \z@ \@plus\z@  \@minus\z@
              \topsep    \parsep
              \itemsep   \parsep}
\def\@listiii{\leftmargin\leftmarginiii
              \labelwidth\leftmarginiii
              \advance\labelwidth-\labelsep
              \parsep    \z@ \@plus\z@  \@minus\z@
              \topsep    \parsep
              \itemsep   \parsep}
\def\@listiv{\leftmargin\leftmarginiv
              \labelwidth\leftmarginiv
              \advance\labelwidth-\labelsep
              \parsep    \z@ \@plus\z@  \@minus\z@
              \topsep    \parsep
              \itemsep   \parsep}
\def\@listv{\leftmargin\leftmarginv
              \labelwidth\leftmarginv
              \advance\labelwidth-\labelsep
              \parsep    \z@ \@plus\z@  \@minus\z@
              \topsep    \parsep
              \itemsep   \parsep}
\def\@listvi{\leftmargin\leftmarginvi
              \labelwidth\leftmarginvi
              \advance\labelwidth-\labelsep
              \parsep    \z@ \@plus\z@  \@minus\z@
              \topsep    \parsep
              \itemsep   \parsep}
%
% Change default margins for \list environments
\setlength\leftmargini   {2\ccwd} \setlength\leftmarginii
{\leftmargini} \setlength\leftmarginiii {\leftmargini}
\setlength\leftmarginiv  {\leftmargini} \setlength\leftmarginv
{\ccwd} \setlength\leftmarginvi  {\ccwd} \setlength\leftmargin
{\leftmargini} \setlength\labelsep      {.5\ccwd}
\setlength\labelwidth    {\leftmargini}
%
\setlength\listparindent{2\ccwd}
% Change \listparindent to 2\ccwd in \list
\def\list#1#2{\ifnum \@listdepth >5\relax \@toodeep
     \else \global\advance\@listdepth\@ne \fi
  \rightmargin \z@ \listparindent2\ccwd \itemindent \z@
  \csname @list\romannumeral\the\@listdepth\endcsname
  \def\@itemlabel{#1}\let\makelabel\@mklab \@nmbrlistfalse #2\relax
  \@trivlist
  \parskip\parsep \parindent\listparindent
  \advance\linewidth -\rightmargin \advance\linewidth -\leftmargin
  \advance\@totalleftmargin \leftmargin
  \parshape \@ne \@totalleftmargin \linewidth
  \ignorespaces}

\makeatother
%---------------------------------------------------
\newcommand\acknowledgement{\chapter*{\ziju{2}致谢}}

%%% ---- 章节标题设置 ----- %%%%
\ctexset{chapter={format+={\zihao{-2}},%
               number={\zihao{-2}\arabic{chapter}},name={,},afterskip={30pt},beforeskip={20pt}}}
\ctexset{section={format+={\zihao{4}\raggedright}}}
\ctexset{subsection={format+={\zihao{-4}\raggedright}}}
\ctexset{subsubsection={format={\zihao{-4}\bf\raggedright}}}

\setcounter{tocdepth}{4}
\setcounter{secnumdepth}{4}

%%% ---- 图表标题设置 ----- %%%%
\RequirePackage[labelsep=quad]{caption} %% 序号之后空一格写标题
\captionsetup[table]{textfont=bf}  %%设置表格标题字体为黑体  。2016.06.07
\renewcommand\figurename{\zihao{-4} 图}
\renewcommand\tablename{\bf\zihao{-4} 表} 

%%% ------ 目录设置 ------- %%%%
\ctexset{contentsname={目\qquad 录}}
 
\RequirePackage{tocloft} 
\renewcommand\cftchapfont{\zihao{4}\bf}
\renewcommand{\cftchapleader}{\cftdotfill{\cftdotsep}}
\renewcommand\cfttoctitlefont{\hfill\zihao{-2}\bf}
\renewcommand\cftaftertoctitle{\hfill}
\newcommand\mydot[1]{\scalebox{#1}{$\cdot$}}
\renewcommand\cftdot{\mydot{1}} % change the size of dots
\renewcommand{\cftdotsep}{2.5}

% \RequirePackage[titles]{tocloft}
% % \renewcommand{\cftdot}{$\cdot$}
% \renewcommand{\cftdotsep}{1.5}
% \setlength{\cftbeforechapskip}{10pt}
% \renewcommand{\cftchapleader}{\cftdotfill{\cftchapdotsep}}
% \renewcommand{\cftchapdotsep}{\cftdotsep}

%%===参考文献===%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\bibliographystyle{abbrv}        % 参考文献样式,  plain,unsrt,alpha,abbrv,chinesebst 等等
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\graphicspath{{figures/}}        % 图片文件路径
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\allowdisplaybreaks

\endlinechar `\^^M
\endinput
