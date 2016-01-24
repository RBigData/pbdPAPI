#!/bin/sh

PKGVER=`grep "Version:" ../DESCRIPTION | sed -e "s/Version: //"`
sed -i -e "s/myversion{.*}/myversion{${PKGVER}}/" pbdPAPI-guide.Rnw

cd cover
./build_cover.sh
cd ..

rm *.aux *.bbl *.blg *.log *.out *.toc
pdflatex pbdPAPI-guide.Rnw
bibtex pbdPAPI-guide
pdflatex pbdPAPI-guide.Rnw
pdflatex pbdPAPI-guide.Rnw
Rscript -e "tools::compactPDF('pbdPAPI-guide.pdf', gs_quality='ebook')"
rm *.aux *.bbl *.blg *.log *.out *.toc *.dvi

#mv -f *.pdf ../inst/doc/
#cp -f *.Rnw ../inst/doc/
