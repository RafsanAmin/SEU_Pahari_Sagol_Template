"""Checks every file imported by the notebook, without needing LaTeX.

- the file exists and KACTL's preprocessor accepts its header
- printed code lines fit the 3-column layout (<= 63 chars, tab = 2)
- Description/Usage text has no characters that break LaTeX
Run from anywhere: python tests/lint.py
"""
import os, re, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
os.chdir(ROOT)
MAXW = 63
errors = []

def strip_math(s):
    s = re.sub(r'\\\[.*?\\\]', '', s, flags=re.S)  # display math
    s = re.sub(r'\\begin\{(array|align\*?|tabular)\}.*?\\end\{\1\}', '', s, flags=re.S)
    return re.sub(r'\$[^$]*\$', '', s)

def strip_bigo(s):  # the preprocessor puts O(...) in math mode
    out, i = '', 0
    while i < len(s):
        if s.startswith('O(', i):
            depth, i = 1, i + 2
            while i < len(s) and depth:
                depth += {'(': 1, ')': -1}.get(s[i], 0); i += 1
        else:
            out += s[i]; i += 1
    return out

def check_text(name, field, text):
    t = strip_math(text)
    if field == 'Time':
        for inner in re.findall(r'O\(([^()]*(?:\([^()]*\)[^()]*)*)\)', t):
            if '\\^' in inner or '\\_' in inner:
                errors.append(f'{name}: Time O(...) is math mode, write ^ and _ without backslash')
        t = strip_bigo(t)
    if field == 'Usage':
        for bad in (r'\_', r'\{', r'\}'):
            if bad in t:
                errors.append(f'{name}: Usage must not contain {bad} (auto-escaped)')
        if re.search(r'(?<!\\)[&%#]', t):
            errors.append(f'{name}: Usage needs \\& \\% \\#')
    else:
        if re.search(r'(?<!\\)[_&%#]', t):
            errors.append(f'{name}: {field} has unescaped _ & % or #')
        if re.search(r'(?<!\\)\^', t):
            errors.append(f'{name}: {field} has ^ outside math (use \\^{{}})')
        m = re.search(r'\\(log|sqrt|cdot|sum|le|ge|leq|geq|alpha|Sigma|times|infty|prod|oplus|lfloor|rfloor)\b', t)
        if m and field != 'Time':
            errors.append(f'{name}: {field} uses \\{m.group(1)} outside math ($...$)')
        if '~' in t:
            errors.append(f'{name}: {field} has ~ (use \\textasciitilde{{}})')
        depth = 0
        for i, c in enumerate(t):
            if c in '{}' and (i == 0 or t[i - 1] != '\\'):
                depth += 1 if c == '{' else -1
                if depth < 0: break
        if depth != 0:
            errors.append(f'{name}: {field} has unbalanced braces')

def header_fields(src):
    m = re.search(r'/\*\*(.*?)\*/', src, re.S)
    if not m: return {}
    fields, cur = {}, None
    for line in m.group(1).split('\n'):
        line = line.strip().lstrip('*').strip()
        mm = re.match(r'([A-Z][a-zA-Z]*):(.*)', line)
        if mm:
            cur = mm.group(1); fields[cur] = mm.group(2).strip()
        elif cur:
            fields[cur] += '\n' + line
    return fields

for chapdir in sorted(os.listdir('content')):
    chap = os.path.join('content', chapdir, 'chapter.tex')
    if not os.path.exists(chap): continue
    tex = open(chap, encoding='utf8').read()
    for opts, f in re.findall(r'^[^%\n]*\\(?:kactl|opt|adv)import(\[[^\]]*\])?\{([^}]*)\}', tex, re.M):
        path = os.path.join('content', chapdir, f)
        name = f'{chapdir}/{f}'
        if not os.path.exists(path):
            errors.append(f'{name}: missing'); continue
        args = [sys.executable, 'content/tex/preprocessor.py', '-i', path, '-o', 'build_lint.tmp']
        if opts: args += opts[1:-1].split()
        subprocess.run(args, stdout=subprocess.DEVNULL, check=True)
        out = open('build_lint.tmp', encoding='utf8').read()
        if r'\kactlerror' in out:
            errors.append(f'{name}: ' + out.split(r'\kactlerror{')[1].split('}')[0])
        code = out.split(r'\begin{lstlisting}', 1)[-1].split('\n', 1)[-1]
        code = code.split(r'\end{lstlisting}')[0]
        for i, line in enumerate(code.split('\n')):
            w = len(line.expandtabs(2))
            if w > MAXW:
                errors.append(f'{name}: printed line {i + 1} is {w} chars: {line.strip()[:50]}')
        if f.endswith('.h'):
            for k, v in header_fields(open(path, encoding='utf8').read()).items():
                if k in ('Description', 'Usage', 'Time'):
                    check_text(name, k, v)
for tmp in ('build_lint.tmp', 'header.tmp'):
    if os.path.exists(tmp): os.remove(tmp)
print('\n'.join(errors) if errors else 'lint: OK')
sys.exit(1 if errors else 0)
