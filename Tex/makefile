DOKUMENT = Ferromagnet
VORTRAG  = Vortrag
HANDOUT  = Handout

PAPER = paper

LATEX = $(DOKUMENT).pdf $(VORTRAG).pdf $(HANDOUT).pdf $(PAPER).pdf

CHAPTERS = chapters/title.tex \
           chapters/introduction.tex \
           chapters/methods.tex\
           chapters/model.tex \
           chapters/results.tex\
           chapters/conclusion.tex\
           chapters/appendix.tex \
           chapters/acknowledgements.tex \
           chapters/erklarung.tex

P_CHAPTERS = paper/introduction.tex \
             paper/methods.tex\
             paper/model.tex \
             paper/results.tex\
             paper/conclusion.tex\
             paper/acknowledgements.tex

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
         plots/GG_Tc_norm_deg_fit.pdf \
         plots/GG_Tc_norm_deg_A0.pdf \
         plots/GG_Tc.pdf \
         plots/GG_Tc_A0.pdf \
         plots/GG_deg.pdf \
         plots/GG_d.pdf \
         plots/GG_sumJ.pdf \
         plots/GG_meanJ.pdf \
         plots/GG_TcG.pdf \
         plots/GGgammaBySus.pdf \

RNG_LST = $(subst GG,RNG,$(GG_LST))

PLOTS = plots/binder_fit_s_0.pdf \
        plots/binder_s_0.pdf \
        plots/collapse_s_0.pdf \
        plots/collapse_s_1_sus.pdf \
        plots/collapse_s_1_meanM.pdf \
        plots/s_1_sus.pdf \
        plots/s_1_meanM.pdf \
        plots/equiE.pdf \
        plots/autoM.pdf \
        plots/meanM.pdf \
        plots/Mean_M_L_128.pdf \
        plots/Specific_Heat_0.pdf \
        plots/autocorrL.pdf \
        plots/autocorrT.pdf \
        plots/Tc_K.pdf \
        plots/Tc_J.pdf \
        plots/Specific_Heat_0_log.pdf \
        $(GG_LST) \
        $(RNG_LST)

P_IMAGES = images/GG_RNG_def.tex

P_GG_LST = plots/GG_Tc.pdf

P_RNG_LST = $(subst GG,RNG,$(P_GG_LST))

P_PLOTS = plots/Binder_over_Correlation_All.pdf \
          $(P_GG_LST) \
          $(P_RNG_LST)

all: $(LATEX)

.PHONY : clean proper

$(DOKUMENT).pdf: $(CHAPTERS)

$(DOKUMENT).pdf $(VORTRAG).pdf $(HANDOUT).pdf: $(PLOTS) $(IMAGES) $(BIBFILE)

$(PAPER).pdf: $(P_CHAPTERS) $(P_PLOTS) $(P_IMAGES) $(BIBFILE)

$(DOKUMENT).pdf $(VORTRAG).pdf $(PAPER).pdf: %.pdf: %.tex
	pdflatex -interaction=batchmode $*
	bibtex $*
	pdflatex -interaction=batchmode $*
	pdflatex -interaction=batchmode $*

$(HANDOUT).pdf: $(VORTRAG).tex
	pdflatex -interaction=batchmode -jobname $(HANDOUT) "\PassOptionsToClass{handout}{beamer}\input{$(VORTRAG)}"
	bibtex $(HANDOUT)
	pdflatex -interaction=batchmode -jobname $(HANDOUT) "\PassOptionsToClass{handout}{beamer}\input{$(VORTRAG)}"
	pdflatex -interaction=batchmode -jobname $(HANDOUT) "\PassOptionsToClass{handout}{beamer}\input{$(VORTRAG)}"

%.eps: %.gp
	cd $(<D) && gnuplot $(<F)

%.pdf: %.eps
	cd $(<D) && epstopdf $(<F)

clean:
	rm -f $(PLOTS) plots/fit.log
	rm -f {$(DOKUMENT),$(VORTRAG),$(HANDOUT)}.{pdf,log,aux,bbl,blg,toc,out,lof,lot,snm,nav}

proper:
	rm -f $(PLOTS) $(P_PLOTS) plots/fit.log
	rm -f {$(DOKUMENT),$(VORTRAG),$(HANDOUT)}.{log,aux,bbl,blg,toc,out,lof,lot,snm,nav}
