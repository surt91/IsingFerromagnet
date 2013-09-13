DOKUMENT = Ferromagnet
VORTRAG  = Vortrag
HANDOUT  = Handout

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
         images/DT_def.tex \
         images/Displacement.tex \
         images/CellSmall.tex \
         images/CellBig.tex \
         images/GGEdgeBefore.tex \
         images/GGEdgeAfter.tex \
         images/parTempSchema.tex \
         images/parTempE.tex \
         images/GG_def.tex \
         images/RNG_def.tex

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

all: $(DOKUMENT).pdf $(VORTRAG).pdf $(HANDOUT).pdf

.PHONY : clean

$(DOKUMENT).pdf: $(DOKUMENT).tex $(CHAPTERS) $(PLOTS) $(IMAGES) $(BIBFILE)
	pdflatex -interaction=batchmode $(DOKUMENT)
	bibtex $(DOKUMENT)
	pdflatex -interaction=batchmode $(DOKUMENT)
	pdflatex -interaction=batchmode $(DOKUMENT)

$(VORTRAG).pdf: $(VORTRAG).tex $(PLOTS) $(IMAGES) $(BIBFILE)
	pdflatex -interaction=batchmode $(VORTRAG)
	bibtex $(VORTRAG)
	pdflatex -interaction=batchmode $(VORTRAG)
	pdflatex -interaction=batchmode $(VORTRAG)

$(HANDOUT).pdf: $(VORTRAG).tex $(PLOTS) $(IMAGES) $(BIBFILE)
	pdflatex -interaction=batchmode -jobname $(HANDOUT) "\PassOptionsToClass{handout}{beamer}\input{$(VORTRAG)}"
	bibtex $(VORTRAG)
	pdflatex -interaction=batchmode -jobname $(HANDOUT) "\PassOptionsToClass{handout}{beamer}\input{$(VORTRAG)}"
	pdflatex -interaction=batchmode -jobname $(HANDOUT) "\PassOptionsToClass{handout}{beamer}\input{$(VORTRAG)}"

%.eps: %.gp
	cd $(<D) && gnuplot $(<F)

%.pdf: %.eps
	cd $(<D) && epspdf -b $(<F)

clean:
	rm -f $(PLOTS) plots/fit.log
	rm -f {$(DOKUMENT),$(VORTRAG),$(HANDOUT)}.{pdf,log,aux,bbl,blg,toc,out,lof,lot,snm,nav}
