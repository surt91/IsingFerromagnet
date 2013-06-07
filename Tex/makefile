DOKUMENT = Ferromagnet
CHAPTERS = chapters/title.tex \
           chapters/introduction.tex \
           chapters/model.tex \
           chapters/monte_carlo.tex

all: $(DOKUMENT).pdf

$(DOKUMENT).pdf: $(DOKUMENT).tex $(CHAPTERS)
	pdflatex $(DOKUMENT)
	bibtex $(DOKUMENT)
	pdflatex $(DOKUMENT)
	pdflatex $(DOKUMENT)

clean:
	rm *.log *.aux *.bbl *.blg *.toc $(DOKUMENT).pdf
