all: presentation.pdf article.pdf

presentation.pdf: presentation.tex
	pdflatex presentation.tex

article.pdf: article.tex
	pdflatex article.tex

clean:
	rm -f *.aux *.pdf *.log *.toc *.nav *.out *.snm

