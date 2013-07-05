DOKUMENT = Ferromagnet
CHAPTERS = chapters/title.tex \
           chapters/introduction.tex \
           chapters/model.tex \
           chapters/monte_carlo.tex\
           chapters/Auswertung.tex

all: $(DOKUMENT).pdf

.PHONY : plots clean

$(DOKUMENT).pdf: $(DOKUMENT).tex $(CHAPTERS) plots
	pdflatex $(DOKUMENT)
	bibtex $(DOKUMENT)
	pdflatex $(DOKUMENT)
	pdflatex $(DOKUMENT)

plots:
	$(MAKE) -C plots

clean:
	rm -rf *.log *.aux *.bbl *.blg *.toc *.out *.lof $(DOKUMENT).pdf
	$(MAKE) clean -C plots
