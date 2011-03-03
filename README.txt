git clone git://github.com/akessel/Vis.git
git config --global user.name "akessel"
git config --global user.email "andrew.kessel@colorado.edu"
git remote add gitVis git@github.com:akessel/Vis.git
git add <file> <file>
git status -s
git commit -a -m 'asd'
git push gitVis master
git rm --cached <file> ("--cached" means that a copy will be kept on the local machine)
