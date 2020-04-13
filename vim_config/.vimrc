set runtimepath+=~/.vim_runtime


source ~/.vim/autoload/plug.vim
source ~/.vim_runtime/vimrcs/basic.vim
source ~/.vim_runtime/vimrcs/filetypes.vim
source ~/.vim_runtime/vimrcs/plugins_config.vim
source ~/.vim_runtime/vimrcs/extended.vim
source ~/.vim_runtime/mru/mru.vim

try
source ~/.vim_runtime/my_configs.vim
catch
endtry

set number


set termguicolors     " enable true colors support
let ayucolor="mirage" " for mirage version of theme
colorscheme ayu


filetype plugin indent on
" show existing tab with 4 spaces width
set tabstop=4
" when indenting with '>', use 4 spaces width
set shiftwidth=4
" On pressing tab, insert 4 spaces
set expandtab


" enable mouse
set mouse=a
