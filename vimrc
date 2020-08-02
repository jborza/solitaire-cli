syntax on
filetype plugin indent on
" expand tab 2 spaces
set expandtab

set tabstop=2
set softtabstop=2
" indenting with >
set shiftwidth=2

autocmd FileType c,cpp setlocal equalprg=clang-format

" map return key in insert mode to produce ctrl+y in (autocomplete) popup menu
inoremap <expr> <CR> pumvisible() ? "\<C-Y>" : "\<CR>"

" tab completion - tab on line beginning, else autocomplete
function! InsertTabWrapper()
  let col = col('.') - 1
  if !col || getline('.')[col - 1] !~ '\k'
    return "\<tab>"
  else
    return "\<c-p>"
  endif
endfunction
inoremap <expr> <tab> InsertTabWrapper()
inoremap <s-tab> <c-n>
