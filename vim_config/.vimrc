set runtimepath+=~/.vim_runtime

" Vundle
" set nocompatible              " be iMproved, required
filetype off                  " required

" set the runtime path to include Vundle and initialize
set rtp+=~/.vim/bundle/Vundle.vim
call vundle#begin()
" alternatively, pass a path where Vundle should install plugins
"call vundle#begin('~/some/path/here')

" let Vundle manage Vundle, required
Plugin 'VundleVim/Vundle.vim'

"Plugin 'rip-rip/clang_complete'
Plugin 'itchyny/lightline.vim'
"Plugin 'neoclide/coc.nvim'
Plugin 'zxqfl/tabnine-vim'
" added nerdtree
Plugin 'scrooloose/nerdtree'

" All of your Plugins must be added before the following line
call vundle#end()            " required
filetype plugin indent on    " required
" To ignore plugin indent changes, instead use:
"filetype plugin on
"
" Brief help
" :PluginList       - lists configured plugins
" :PluginInstall    - installs plugins; append `!` to update or just :PluginUpdate
" :PluginSearch foo - searches for foo; append `!` to refresh local cache
" :PluginClean      - confirms removal of unused plugins; append `!` to auto-approve removal
"
" see :h vundle for more details or wiki for FAQ
" Put your non-Plugin stuff after this line
"
" clang_complete
" path to directory where library can be found
 let g:clang_library_path='/usr/lib/llvm-6.0/lib'
 " or path directly to the library file
 let g:clang_library_path='/usr/lib/x86_64-linux-gnu/libclang-6.0.so.1'

source ~/.vim/autoload/plug.vim
"source ~/.vim_runtime/vimrcs/coc.vim
source ~/.vim_runtime/vimrcs/basic.vim
source ~/.vim_runtime/vimrcs/filetypes.vim
source ~/.vim_runtime/vimrcs/plugins_config.vim
"source ~/.vim_runtime/vimrcs/extended.vim
source ~/.vim_runtime/mru/mru.vim

try
source ~/.vim_runtime/my_configs.vim
catch
endtry



" enable line numbers
set number



set termguicolors     " enable true colors support
let ayucolor="mirage" " for mirage version of theme
colorscheme ayu


filetype plugin indent on
" show existing tab with 4 spaces width
set tabstop=2
" when indenting with '>', use 4 spaces width
set shiftwidth=2
" On pressing tab, insert 4 spaces
set expandtab


" enable mouse
set mouse=a

" relative line numbers useful for multiline operations
set relativenumber

" the following script is for
" do not yank when putting 
" thanks to https://stackoverflow.com/questions/290465/how-to-paste-over-without-overwriting-register
" I haven't found how to hide this function (yet)
function! RestoreRegister()
  let @" = s:restore_reg
  return ''
endfunction

function! s:Repl()
    let s:restore_reg = @"
    return "p@=RestoreRegister()\<cr>"
endfunction

" NB: this supports "rp that replaces the selection by the contents of @r
vnoremap <silent> <expr> p <sid>Repl()

"let NERDTreeWinSize = 15
nmap <F6> :NERDTreeToggle<CR>

let NERDTreeShowHidden=1

set clipboard=unnamedplus

