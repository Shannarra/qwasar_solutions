counter=0

while getopts ":f:n:d:" opt; do
  case $opt in
    f)
      files_name=$OPTARG
      # echo "-f was triggered, Parameter: $OPTARG" >&2
      ;;
    n)
      folders_c=$OPTARG
      # echo "-n for: $OPTARG" >&2
      ;;
    d)
      if [ $OPTARG -ge 1 ]; then
        delete_existing="ON"
        echo "\e[33m [WARN!] Deleting folers if existing is ON!"
      else
        delete_existing="OFF"
      fi 

      ;;
    \?)
      echo "Invalid option: -$OPTARG" >&2
      echo "Valid arguments: -f [FILENAME], -n NUM_OF_FOLDRS"
      exit 1
      ;;
    :)
      echo "Option -$OPTARG requires an argument." >&2
      exit 1
      ;;
  esac
done


while [ $counter -le $folders_c ]
do

    if [ "$delete_existing" = "ON" ]; then
        if [ -d "./ex0$counter" ]; then
	    __name="./ex0$counter"
            echo "\033[31m DELETING FOLDER \"$__name\" AND ITS CONTENTS!"
            rm -rf $__name
        fi
    fi

    
    if [ "$counter" -gt "9" ]; then
      mkdir "./ex$counter"
      cd "./ex$counter"
    else    
      mkdir "./ex0$counter"
      cd "./ex0$counter"
    fi

    if [ $files_name ]; then
        touch "$files_name"
        cd ".."
    fi


    counter=$((counter+1))
done


echo "\033[32m Operation successful! ;)"
