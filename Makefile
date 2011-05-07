NAME=reference

view: $(NAME).pdf
	evince $(NAME).pdf

$(NAME).pdf: $(NAME).tex src/*.cpp
	pdflatex $(NAME).tex
	pdflatex $(NAME).tex

.PHONY: clean mrproper

mrproper: clean
	rm -rf $(NAME).pdf

clean:
	rm -rf $(NAME).log $(NAME).aux $(NAME).toc $(NAME).dvi

