path=$(dirname $(realpath $0))
nvim_config_path="${HOME}/.config/nvim/plugin"

if [[ -d $nvim_config_path ]]
then
    for file in *.vim
    do
        ln $file $nvim_config_path
    done
else
    echo "not found"
fi


