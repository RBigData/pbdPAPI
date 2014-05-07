#!/bin/sh

rm *.aux *.bbl *.blg *.log *.out *.toc
pdflatex pbdPAPI-guide.Rnw
bibtex pbdPAPI-guide
pdflatex pbdPAPI-guide.Rnw
pdflatex pbdPAPI-guide.Rnw
Rscript -e "tools::compactPDF('pbdPAPI-guide.pdf', gs_quality='ebook')"
rm *.aux *.bbl *.blg *.log *.out *.toc *.dvi

mv -f *.pdf ../inst/doc/
cp -f *.Rnw ../inst/doc/
