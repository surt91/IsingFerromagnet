DOKUMENT = Ferromagnet

all: $(DOKUMENT).pdf

$(DOKUMENT).pdf: $(DOKUMENT).tex
	pdflatex $(DOKUMENT)
	bibtex $(DOKUMENT)
	pdflatex $(DOKUMENT)
	pdflatex $(DOKUMENT)

clean:
	rm *.log *.aux *.bbl *.blg *.toc $(DOKUMENT).pdf
