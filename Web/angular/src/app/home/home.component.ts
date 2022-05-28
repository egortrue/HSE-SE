import { Component } from '@angular/core';
import { RECIPES } from '../recipe/recipe.data';

@Component({
  selector: 'app-home',
  templateUrl: './home.component.html',
  styleUrls: ['./home.component.css'],
})
export class HomeComponent {
  recipesCount: number;
  bestRecipeID: number;
  newRecipeID: number;

  constructor() {
    this.recipesCount = RECIPES.length;
    this.bestRecipeID = Math.floor(Math.random() * this.recipesCount);
    this.newRecipeID = Math.floor(Math.random() * this.recipesCount);
    if (this.bestRecipeID == this.newRecipeID) {
      if (this.bestRecipeID == 0) {
        this.newRecipeID += 1;
      } else {
        this.newRecipeID -= 1;
      }
    }
  }
}
