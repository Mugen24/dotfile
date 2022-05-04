#Get abs path of file then retrieve dir
file_path=$(dirname $(realpath $0))
s_config="$HOME/.config/sway/config"
zsh_config="$HOME/.zshrc"
#Programs

#Install packages
for p in $(cat $file_path/requirements.txt)
do
    echo cum >/dev/null
    #sudo pacman -S --needed $p
    reboot
done

#vimrc
if [[ ! -d $HOME/.vim_runtime ]]
then
    git clone --depth=1 https://github.com/amix/vimrc.git ~/.vim_runtime
    sh ~/.vim_runtime/install_awesome_vimrc.sh

    echo "
    inoremap jj <Esc>
    " > ~/.vim_runtime/my_configs.vim
fi

#sway
if [[ -f $s_config ]]
then
    echo -e "\ninclude $file_path/sway/my_configs" >> $s_config
    #Enable brightness bar(wob)
    cp contrib/systemd/wob.{service,socket} ~/.local/share/systemd/user/
systemctl daemon-reload --user

    systemctl enable --now --user wob.socket

else
    cp $file_path/sway/.default_sway.conf $s_config
fi

#zsh
if [[ ! -a "$HOME/.oh-my-zsh" ]]
then
    sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)"
else
    if [[ -a $zsh_config ]]
    then
        echo -e "\nsource $file_path/zsh/my_configs" >> $zsh_config
        echo -e "\nsource $file_path/zsh/git_alias" >> $zsh_config
    else
        echo "Please install zsh"
    fi
fi

#qute-browser
