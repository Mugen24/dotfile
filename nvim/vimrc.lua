vim.api.nvim_set_keymap('n', 'S', ':!alacritty --working-directory $(pwd) &', {})

vim.api.nvim_set_keymap('n', '<A-1>', ':silent! tabn 1 <CR>', {})
vim.api.nvim_set_keymap('n', '<A-2>', ':silent! tabn 2 <CR>', {})
vim.api.nvim_set_keymap('n', '<A-3>', ':silent! tabn 3 <CR>', {})
vim.api.nvim_set_keymap('n', '<A-4>', ':silent! tabn 4 <CR>', {})
vim.api.nvim_set_keymap('n', '<A-5>', ':silent! tabn 5 <CR>', {})
vim.api.nvim_set_keymap('n', '<A-6>', ':silent! tabn 6 <CR>', {})
vim.api.nvim_set_keymap('n', '<A-7>', ':silent! tabn 7 <CR>', {})
vim.api.nvim_set_keymap('n', '<A-8>', ':silent! tabn 8 <CR>', {})
vim.api.nvim_set_keymap('n', '<A-9>', ':silent! tabn $ <CR>', {})

--vim.api.nvim_set_keymap('n', '<C-w>', ':silent! tabc <CR>', {})
