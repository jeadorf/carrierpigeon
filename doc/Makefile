all: presentation.pdf article.pdf

presentation.pdf: presentation.tex
	pdflatex presentation.tex

article.pdf: article.tex
	pdflatex article.tex

clean:
	rm *.aux
	rm *.pdf
	rm *.log
	rm *.toc
	rm *.nav
	rm *.out
	rm *.sm,

