import importlib.util
for m in ['fitz','pypdf','PyPDF2','pdfminer','pdfplumber']:
    print(' ', m, 'OK' if importlib.util.find_spec(m) else 'MISSING')
