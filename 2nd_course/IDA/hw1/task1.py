def separate_for_sentences(text: str):
    end = set()
    end.add("!")
    end.add("?")
    sentences_and_eos = dict()
    sentence = ""
    flag_triple_dot = 1

    for index, element in enumerate(text):
        if flag_triple_dot == 2:
            flag_triple_dot += 1
            continue
        if flag_triple_dot == 3:
            flag_triple_dot = 1
            continue
        if element in end:
            sentences_and_eos[sentence] = element
            sentence = ""
        elif element == ".":
            try:
                if text[index + 1] == "." and text[index + 2] == ".":
                    sentences_and_eos[sentence.strip()] = element + text[index + 1] + text[index + 2]
                    sentence = ""
                    flag_triple_dot += 1
                else:
                    sentences_and_eos[sentence.strip()] = element
                    sentence = ""
            except IndexError:
                sentences_and_eos[sentence.strip()] = element
                sentence = ""
        elif element == ",":
            sentence += ""
        else:
            sentence += element
    return sentences_and_eos


def count_words(sentences_and_eos: dict):
    counter = 0
    for sentence in sentences_and_eos:
        if sentence != "":
            counter += len(sentence.split(" "))
    return counter


def reverse_words_in_sentences(sentences_and_eos: dict):
    reversed_sentences = [" ".join(sentence.split(" ")[::-1]) + ending for sentence, ending in sentences_and_eos.items()]
    return " ".join(reversed_sentences)


def top_10_repeat_words(sentences_and_eos: dict):
    words = dict()
    for sentence in sentences_and_eos:
        for word in sentence.split(" "):
            if word in words:
                words[word] += 1

            else:
                words[word] = 1
    words = sorted(words, key=words.get, reverse=True)
    return [word for word in words][:10]


with open("lorem_ipsum.txt", "r") as f:
    data_file = [line.strip() for line in f.readlines()]  # delete special symbol (\n,\t ...)
data_file = "".join(data_file)
sentences_and_eos = separate_for_sentences(data_file)
print("Count of words:", count_words(sentences_and_eos))
print("Top 10 words:", ", ".join(top_10_repeat_words(sentences_and_eos)))
print("Reversed text:", reverse_words_in_sentences(sentences_and_eos))
