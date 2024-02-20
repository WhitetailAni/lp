port="/dev/ttyS3"

while [[ $# -gt 0 ]]; do
  case "$1" in
    --print)
      shift
      
      if [[ "$#" -gt 0 ]]; then
        echo "$*" > port
      else
        echo "w-where's the text?"
        exit 1
      fi
      exit 1
      ;;
    --file)
      shift

      if [[ "$#" -gt 0 ]]; then
        cat "$1" > port
      else
	echo "i need a file...."
      fi
      exit 1
      ;;
    --cut)
      echo -e "\x1D""V\x00" > port
      exit 1
      ;;
    --newline)
      echo -e "\n" > port
      exit 1
      ;;
    --newlines)
      if [[ "$2" =~ ^[0-9]+$ ]]; then
        for ((i=0; i<$2; i++)); do
          echo -e "\n" > port
        done
        shift
      else
        echo -n "how many lines senpai?"
      fi
      exit 1
      ;;
    --newcut)
      echo -e "\n" > port
      echo -e "\n" > port
      echo -e "\x1D""V\x00" > port
      exit 1
      ;;
    --image)
      shift

      if [[ "$#" -gt 0 ]]; then
        if [ -e "$1" ]; then
          /bin/bitmap "$1" > port
        else
          echo "that image doesnt exist."
        fi
      else
        echo "i need an image...."
      fi
      exit 1
      ;;
    --help)
      echo "--print will print any text u give me"
      echo "--file will print the contents of any file u give me"
      echo "--cut will cut the printout so u can see it"
      echo "--newline will print a single new line"
      echo "--newlines will print N newlines"
      echo "--help will print this again!!!!"
      exit 1
      ;;
    *)
      echo "thats not a valid command >_<"
      exit 1
      ;;
  esac
  shift
done

if [ "$#" -eq 0 ]; then
	echo "what should I do mommy? tell me >_<"
fi
