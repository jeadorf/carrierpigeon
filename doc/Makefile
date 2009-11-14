
all: presentation.pdf article.pdf

presentation.pdf: presentation.tex
	pdflatex presentation.tex

article.pdf: article.tex stripheaders 
	pdflatex article.tex

# TODO: copy to sub-directory e.g. .build
stripheaders:
	find ../letterbox -name "*.h" | ./stripheaders.py
	echo "../letterbox/main.c" | ./stripheaders.py -static

clean:
	rm -f *.aux *.pdf *.log *.toc *.nav *.out *.snm *.h

