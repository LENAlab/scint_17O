(TeX-add-style-hook "notes"
 (lambda ()
    (LaTeX-add-labels
     "sec-1"
     "sec-2"
     "sec-3")
    (TeX-run-style-hooks
     "enumitem"
     "times"
     "fullpage"
     "hyperref"
     "amssymb"
     "wasysym"
     "marvosym"
     "textcomp"
     "amsmath"
     "ulem"
     "normalem"
     "rotating"
     "wrapfig"
     "float"
     "longtable"
     "graphicx"
     "fixltx2e"
     ""
     "fontenc"
     "T1"
     "inputenc"
     "utf8"
     "latex2e"
     "art11"
     "article"
     "11pt")))

