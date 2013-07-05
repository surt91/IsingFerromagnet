GG_LST = GG_Tc.pdf \
         GG_deg.pdf \
         GG_sumJ.pdf \
         GG_meanJ.pdf
RNG_LST = $(subst GG,RNG,$(GG_LST))
PLOTS = binder_fit_s_0.pdf \
        $(GG_LST) \
        $(RNG_LST)

all: $(PLOTS)

%.eps: %.gp
	gnuplot $<

%.pdf: %.eps
	epstopdf $<

clean:
	rm -f $(PLOTS) *.eps
	rm -rf data
