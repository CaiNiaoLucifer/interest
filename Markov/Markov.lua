-- read input line and split into words for return

function returnWords ()

    local line = io.read()
    local pos = 1

    return function ()

        while line do

            local _start , _end = string.find(line , "%w+" , pos)

            if(_start) then

                pos = _end + 1
                return string.sub(line , _start , _end)

            else

                line = io.read()
                pos = 1

            end
        end

        return nil

    end
end

--generate a string in this style "w1 w2" (note: there is a blank between w1 and
--w2)

function genpairs(w1 , w2)

    return (w1.." "..w2)

end

stattable = {} --a global var which stands for the vocabulary table

--insert (key ,values) in stattable

function insert(index , value)

    if not stattable[index] then

        stattable[index] = {value}
    else

        table.insert(stattable[index] , value)

    end
end

--build stattable

function init()

    local placeholers1 = "#"
    local placeholers2 = "#"

    for value in returnWords() do

        key = genpairs(placeholers1 , placeholers2)

        insert(key , value)

        placeholers1 = placeholers2
        placeholers2 = value

    end

    insert(genpairs(placeholers1 , placeholers2) , "#")
end

--return num of elements of a table

function getn(table)

    local MAXNUM = 100
    local count = 0

    for num = 1 , MAXNUM do

        if table[num] ~= nil then

           count = count + 1

        else

            break;

        end
    end

    return count

end

--generate text

local MAXWORDS = 100
local word1 = "#"
local word2 = "#"

init()

for i = 1 , MAXWORDS do

    local textunit = stattable[genpairs(word1 , word2)]
    local num = getn(textunit)

    local xx = math.random(num)

    local textword = textunit[xx]

    if "#" ~= textword then

        io.write(textword , " ")
        word1 = word2
        word2 = textword

    else

        io.write("\n")
        break

    end
end
