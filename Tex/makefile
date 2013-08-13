DOKUMENT = Ferromagnet
CHAPTERS = chapters/title.tex \
           chapters/introduction.tex \
           chapters/methods.tex\
           chapters/model.tex \
           chapters/results.tex\
           chapters/conclusion.tex\
           chapters/appendix.tex \
           chapters/acknowledgements.tex \
           chapters/erklarung.tex

BIBFILE = lit.bib

IMAGES = images/GG_RNG_def.tex \
         images/Displacement.tex

GG_LST = plots/GG_Tc_normJ.pdf \
         plots/GG_Tc_norm_deg.pdf \
         plots/GG_Tc_norm_deg_A0.pdf \
         plots/GG_Tc.pdf \
         plots/GG_Tc_A0.pdf \
         plots/GG_deg.pdf \
         plots/GG_sumJ.pdf \
         plots/GG_meanJ.pdf \
         plots/GG_TcG.pdf \
         plots/GGgammaBySus.pdf \

RNG_LST = $(subst GG,RNG,$(GG_LST))
PLOTS = plots/binder_fit_s_0.pdf \
        plots/binder_s_0.pdf \
        plots/collapse_s_0.pdf \
        plots/collapse_s_0_sus.pdf \
        plots/collapse_s_1_sus.pdf \
        plots/collapse_s_0_meanM.pdf \
        plots/collapse_s_1_meanM.pdf \
        plots/s_0_sus.pdf \
        plots/s_1_sus.pdf \
        plots/s_0_meanM.pdf \
        plots/s_1_meanM.pdf \
        plots/equiE.pdf \
        plots/autoM.pdf \
        plots/meanM.pdf \
        plots/Mean_M_L_128.pdf \
        plots/Specific_Heat_0.pdf \
        plots/autocorrL.pdf \
        plots/autocorrT.pdf \
        $(GG_LST) \
        $(RNG_LST)

all: $(DOKUMENT).pdf

.PHONY : clean

$(DOKUMENT).pdf: $(DOKUMENT).tex $(CHAPTERS) $(PLOTS) $(IMAGES) $(BIBFILE)
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
	rm -f $(PLOTS) plots/fit.log
	rm -f *.log *.aux *.bbl *.blg *.toc *.out *.lof *.lot $(DOKUMENT).pdf
