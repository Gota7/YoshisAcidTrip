docker run --env-file envs/$1.env -v "$(pwd):/ReunificationX" --rm -it $(docker build -q . )
sudo chown -R $USER:$USER build
sudo chown -R $USER:$USER bin