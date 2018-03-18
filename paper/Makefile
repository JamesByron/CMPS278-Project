#
# Basic LaTeX makefile
#
# Default target: PDF version of the paper
# make clean: removes "temporary" files
# make spotless: clean + removes the default target, forcing rebuild
#

# This is your "main" LaTeX file, without the ".tex"
PAPER      = sample

# Put other LaTeX sources here (excluding the "main" LaTeX file)
TEXSRCS    =

# Put the list of bibliography files here
BIBS       = sample.bib

# This is the name of the automatic LaTeX make program.
LATEXMK    = latexmk

# This needs to be the first target so that it's the one made if you just type make
default: pdf

# Additional dependencies (figures, graphs, etc.) can go below this line

#----------------------------------------------------------------------
# Shouldn't need to edit below this line
#----------------------------------------------------------------------
.SUFFIXES:	.pdf .ps .dvi .tex .eps

pdf: $(PAPER).pdf

$(PAPER).pdf: $(TEXSRCS) $(BIBS)

%.pdf: %.tex
	$(LATEXMK) -pdf $<

clean:
	$(RM) *.aux *.bbl *.blg *.dvi *.log *.out *.fdb_latexmk *.fls *~

spotless: clean
	$(RM) $(PAPER).pdf
