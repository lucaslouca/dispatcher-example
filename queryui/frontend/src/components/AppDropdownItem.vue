<template>
  <span>
    <slot name="activator" :onClick="onClick" />
  </span>
</template>

<script setup lang="ts">
function onClick() {
  alert('appdropwdownitem')
}
</script>

<style lang="scss" scoped>
span {
  font-family: 'Open Sans', 'Helvetica Neue', 'Segoe UI', 'Calibri', 'Arial', sans-serif;
  display: block;
  width: 100%;
  line-height: 2rem;
  text-decoration: none;
  background-color: white;
  box-shadow: rgba(0, 0, 0, 0.04) 0px 3px 5px;
  margin-bottom: 5px;
  height: 2rem;
  transition: all 0.2s;
  cursor: pointer;
  border-radius: 5px;
}

span:hover {
  background-color: rgb(247, 247, 247);
}
</style>
