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
             paper/model.tex \
             paper/results.tex\
             paper/conclusion.tex\
             paper/acknowledgements.tex

BIBFILE = lit.bib

TIKZ = images/GG_RNG_def.pdf \
       images/DT_def.pdf \
       images/Displacement.pdf \
       images/CellSmall.pdf \
       images/CellBig.pdf \
       images/GGEdgeBefore.pdf \
       images/GGEdgeAfter.pdf \
       images/parTempSchema.pdf \
       images/parTempE.pdf \
       images/GG_def.pdf \
       images/RNG_def.pdf

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

P_TIKZ = images/GG_RNG_def.pdf \
         images/GGEdgeAfter.pdf \

P_TEX_PLOTS = plots/col_s1_sus.eps \
              plots/col_s1_m.eps \
              plots/Binder_over_Correlation_All.eps \
              plots/binder.eps \
              plots/Tc_RNG.eps \
              plots/Tc_GG.eps \
              plots/Tc_both.eps \
              plots/TcK.eps \
              plots/K_sigma_inset_both.eps

P_PLOTS = $(P_TEX_PLOTS)
P_TIKZ = $(TIKZ:.pdf=.eps)

all: $(LATEX)

.PHONY : clean proper

$(DOKUMENT).pdf: $(CHAPTERS)

$(DOKUMENT).pdf $(VORTRAG).pdf $(HANDOUT).pdf: $(PLOTS) $(TIKZ) $(BIBFILE)

$(PAPER).pdf: $(P_CHAPTERS) $(P_PLOTS) $(P_TIKZ) $(BIBFILE)

$(DOKUMENT).pdf $(VORTRAG).pdf: %.pdf: %.tex
	pdflatex -interaction=batchmode $*
	bibtex $*
	pdflatex -interaction=batchmode $*
	pdflatex -interaction=batchmode $*

$(PAPER).pdf: %.pdf: %.tex
	latex -interaction=batchmode $*
	bibtex $*
	latex -interaction=batchmode $*
	latex -interaction=batchmode $*
	dvips $*.dvi > /dev/null  2>&1
	ps2pdf $*.ps > /dev/null

$(HANDOUT).pdf: $(VORTRAG).tex
	pdflatex -interaction=batchmode -jobname $(HANDOUT) "\PassOptionsToClass{handout}{beamer}\input{$(VORTRAG)}"
	bibtex $(HANDOUT)
	pdflatex -interaction=batchmode -jobname $(HANDOUT) "\PassOptionsToClass{handout}{beamer}\input{$(VORTRAG)}"
	pdflatex -interaction=batchmode -jobname $(HANDOUT) "\PassOptionsToClass{handout}{beamer}\input{$(VORTRAG)}"

%.eps: %.gp
	cd $(<D) && gnuplot $(<F) > /dev/null 2>&1

%.tex: %.gp
	cd $(<D) && gnuplot $(<F) > /dev/null 2>&1

%.pdf: %.eps
	cd $(<D) && epspdf -b $(<F)

$(P_TIKZ) $(P_TEX_PLOTS): %.eps: %.tex
	cd $(<D) && pdflatex -interaction=batchmode $(<F) > /dev/null
	pdftops -eps $*.pdf > /dev/null 2>&1
	rm -f $*.{log,aux}

clean: proper
	rm -f $(PLOTS) plots/fit.log
	rm -f {$(DOKUMENT),$(VORTRAG),$(HANDOUT)}.pdf

proper:
	rm -f $(PLOTS) $(P_PLOTS) plots/fit.log
	rm -f $(TIKZ) $(P_TIKZ)
	rm -f plots/*{-inc.eps,-inc-eps-converted-to.pdf,.tex,.dvi,.pdf,.eps}
	rm -f {$(DOKUMENT),$(VORTRAG),$(HANDOUT)}.{log,aux,bbl,blg,toc,out,lof,lot,snm,nav}
