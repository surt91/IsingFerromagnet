DOKUMENT = Ferromagnet
CHAPTERS = chapters/title.tex \
           chapters/introduction.tex \
           chapters/model.tex \
           chapters/monte_carlo.tex\
           chapters/Auswertung.tex

GG_LST = plots/GG_Tc.pdf \
         plots/GG_deg.pdf \
         plots/GG_sumJ.pdf \
         plots/GG_meanJ.pdf
RNG_LST = $(subst GG,RNG,$(GG_LST))
PLOTS = plots/binder_fit_s_0.pdf \
        plots/collapse_s_0.pdf \
        plots/equiE.pdf \
        plots/autoM.pdf \
        $(GG_LST) \
        $(RNG_LST)

all: $(DOKUMENT).pdf

.PHONY : clean

$(DOKUMENT).pdf: $(DOKUMENT).tex $(CHAPTERS) $(PLOTS)
	pdflatex -interaction=batchmode $(DOKUMENT)
	bibtex $(DOKUMENT)
	pdflatex -interaction=batchmode $(DOKUMENT)
	pdflatex -interaction=batchmode $(DOKUMENT)

%.eps: %.gp
	cd $(<D) && gnuplot $(<F)
	cd $(@D) && ps2epsi $(@F) $(@F)_tmp && mv $(@F)_tmp $(@F)

%.pdf: %.eps
	cd $(<D) && epstopdf $(<F)

clean:
	rm -f $(PLOTS)
	rm -rf $(dir $(DATA))
	rm -f *.log *.aux *.bbl *.blg *.toc *.out *.lof $(DOKUMENT).pdf
