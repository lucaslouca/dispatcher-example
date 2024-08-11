<template>
  <div class="container">
    <template v-if="strat">
      <h2>{{ strategy.name }}</h2>
      <div class="query-box">
        <template v-if="!strategy.input">
          <!-- Simple Parameters -->
          <span
            v-for="(parameter, index) in strategy.parameters"
            v-bind:key="`row-${index}`"
            class="row"
          >
            <label :for="`input-${parameter}`">{{ parameter }}</label>
            <input
              type="text"
              :id="`input-${parameter}`"
              class="input-field"
              v-model="parameters.SearchFor[parameter]"
              @change="updateUsername"
            />
          </span>

          <!-- Repeated 
        'repeated': ['username', 'age'] 
        -->
          <section
            v-for="[key, values] in repeatedParameters"
            v-bind:key="`section-${key}`"
            class="repeated-section"
          >
            <section
              class="row repeated"
              v-for="(parameterValue, index) in values"
              v-bind:key="`section-row-${key}-${index}`"
            >
              <label :for="`input-${key}`">{{ key }} {{ index }}</label>
              <input
                type="text"
                :id="`repeated-input-${key}`"
                class="input-field"
                v-model="values[index]"
                @change="updateUsername"
              />

              <button @click="removeRepeatedField(key, index)">
                <font-awesome-icon icon="fa-solid fa-minus" />
              </button>
            </section>
            <button @click="addRepeatedField(key)">
              <font-awesome-icon icon="fa-solid fa-plus" />
            </button>
          </section>
        </template>
        <!-- CSV 
        'input': 'csv' 
        -->
        <section v-if="strategy.input == 'csv'">
          <input type="file" id="fileinput" @change="readCsvFile" />
          {{ csvJson }}
        </section>

        <button
          class="my-super-cool-btn"
          @click="onStartButtonClick"
          :disabled="username.length == 0"
        >
          <span>Run</span>
        </button>
      </div>
    </template>
  </div>
</template>

<script setup lang="ts">
import { API } from '@/api'
import { ref, onMounted, watch } from 'vue'
import { useRouter } from 'vue-router'
import { loadState, SearchParameters } from '@/common'
import type { State } from '@/common'

const props = defineProps<{
  strategy: object
}>()

const router = useRouter()
let stateKey = 'query-state'
const state = loadState(stateKey)
const username = ref(state.name)

let strat = ref(props.strategy)
let parameters = ref<SearchParameters>(new SearchParameters())
let repeatedParameters = ref<Map<string, Array<string>>>(new Map<string, Array<string>>())
let csvJson = ref<string>('')

onMounted(() => {
  resetRepeatedParameters()
})

watch(
  () => props.strategy,
  (newValue, oldValue) => {
    strat.value = newValue
    parameters.value = new SearchParameters()

    resetRepeatedParameters()
  }
)
/********************************/
/* REPEATED FIELDS */
/********************************/
function addRepeatedField(fieldName: string) {
  repeatedParameters.value.get(fieldName)?.push('new ' + fieldName)
}

function removeRepeatedField(fieldName: string, index: number) {
  console.log('Remove ' + fieldName + ' at index ' + index)
  repeatedParameters.value.get(fieldName)?.splice(index, 1) // 2nd parameter means remove one item only
}

function resetRepeatedParameters() {
  repeatedParameters.value = new Map<string, Array<string>>()
  if (strat.value.repeated) {
    strat.value.repeated.forEach((rp, i) => {
      repeatedParameters.value.set(rp, ['test'])
    })
  }
}

/********************************/

/********************************/
/* INPUT: CSV */
/********************************/
function readCsvFile(evt) {
  var f = evt.target.files[0]
  if (f) {
    var r = new FileReader()
    r.onload = function (e) {
      var contents = e.target.result
      // csvJson.value = csvJSON(contents)

      var lines = contents.split('\n')

      repeatedParameters.value = new Map<string, Array<string>>()
      let header = lines[0].split(';')
      for (var i = 0; i < header.length; i++) {
        repeatedParameters.value.set(header[i], [])
      }

      for (var l = 1; l < lines.length; l++) {
        let currentLine = lines[l].split(';')
        for (var c = 0; c < currentLine.length; c++) {
          repeatedParameters.value.get(header[c])?.push(currentLine[c])
        }
      }
      console.log(repeatedParameters.value)
    }
    r.readAsText(f)
  } else {
    alert('Failed to load file')
  }
}

//var csv is the CSV file with headers
function csvJSON(csv: any) {
  var lines = csv.split('\n')

  var result = []

  // NOTE: If your columns contain commas in their values, you'll need
  // to deal with those before doing the next step
  // (you might convert them to &&& or something, then covert them back later)
  // jsfiddle showing the issue https://jsfiddle.net/
  var headers = lines[0].split(',')

  for (var i = 1; i < lines.length; i++) {
    var obj = {}
    var currentline = lines[i].split(',')

    for (var j = 0; j < headers.length; j++) {
      obj[headers[j]] = currentline[j]
    }

    result.push(obj)
  }

  //return result; //JavaScript object
  return JSON.stringify(result) //JSON
}
/********************************/

function updateUsername() {
  updateState({ name: username.value })
}

function updateState(newState: Partial<State>): void {
  const state = loadState(stateKey)
  const updatedState = { ...state, ...newState }
  localStorage.setItem(stateKey, JSON.stringify(updatedState))
}

function goToDashboard() {
  router.push({ path: '/dashboard' })
}

function onStartButtonClick() {
  runStrategy({ name: strat.value.name, parameters: parameters })
  // start()
}

function runStrategy(strategy: { name: string; parameters: SearchParameters }) {
  let data = {
    name: strategy.name,
    parameters: strategy.parameters.value.SearchFor
  }

  if (strat.value.repeated || (strat.value.input && strat.value.input === 'csv')) {
    const mapToObject = (map) => Object.fromEntries(map.entries())
    data['repeated'] = mapToObject(repeatedParameters.value)
  }

  API.getEndpoints('strategy')!.get('create')!(data).then(function (response: any) {
    console.log('Get run strategy response', response)
    if (response.data.success) {
      goToDashboard()
    }
  })
}
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style lang="scss" scoped>
* {
  font-family: 'Product Sans', sans-serif;
}
h2 {
  font-size: 2.5rem;
  margin-bottom: 2rem;
}
/***********************************************************************************/
/* QUERY INPUT */
/***********************************************************************************/

input.input-field {
  display: block;
  width: 100%;
  margin-bottom: 0.5em;
  box-sizing: border-box;
  padding: 1em;
  border: 3px rgba(255, 255, 255, 0.9) solid;
  border-radius: 300px;
  font-size: 1em;
  background-color: #e3ded9;
  box-shadow: inset 0 0 3px 1px rgba(0, 0, 0, 0.07);
}

input.input-field:focus {
  outline: none;
  background-color: #beb9b3;
  border: 3px #94c2ed solid;
}

.query-box {
  display: grid;
  grid-template-columns: auto;
}

.query-box .repeated-section {
  padding-top: 0.5rem;
  margin-bottom: 0.5rem;
  border-top: 1px #c7c7c7 solid;
}
.query-box .row {
  display: grid;
  grid-template-columns: 40% auto;
  align-items: center;
}

.query-box .row.repeated {
  display: grid;
  grid-template-columns: 40% 1fr 30px;
  align-items: center;
}

.query-box .row label {
  text-transform: capitalize;
  text-align: left;
  padding-right: 1rem;
}

/*********************************************************/
.my-super-cool-btn {
  letter-spacing: 1px;
  font-size: 1rem;
  padding: 1rem;
  width: 100%;
  border: 2px solid $gray;
  border-radius: 300px;
  box-sizing: border-box;
  transition: all 0.5s cubic-bezier(0.25, 1, 0.33, 1);
  box-shadow:
    0 30px 85px rgba(0, 0, 0, 0.14),
    0 15px 35px rgba(0, 0, 0, 0.14);
  background: rgb(255, 255, 255);
  background: linear-gradient(180deg, rgba(255, 255, 255, 1) 0%, rgba(249, 249, 249, 1) 100%);
  cursor: pointer;
  margin-top: 2rem;
}

.my-super-cool-btn span {
  font-family: 'Montserrat', sans-serif;
  background-color: $gray;
  color: transparent;
  text-shadow: 1px 1px 1px rgba(255, 255, 255, 0.3);
  -webkit-background-clip: text;
  -moz-background-clip: text;
  background-clip: text;
  padding: 5px;
  clip-path: inset(2px);
}

.my-super-cool-btn:hover {
  transform: scale(0.95);
  box-shadow:
    0 20px 55px rgba(0, 0, 0, 0.14),
    0 15px 35px rgba(0, 0, 0, 0.14);
}

button:disabled,
button[disabled] {
  background: white;
  border: 1px solid rgb(208, 208, 208);
  color: gray;
  box-shadow: none;
}

button:disabled span,
button[disabled] span {
  background: white;
  color: rgb(234, 234, 234);
  box-shadow: none;
  text-shadow: none;
  -webkit-background-clip: none;
  -moz-background-clip: none;
  background-clip: none;
  padding: 0;
  clip-path: none;
}

button:disabled:hover,
button[disabled]:hover {
  box-shadow: none;
  transform: none;
  cursor: not-allowed;
}
</style>
