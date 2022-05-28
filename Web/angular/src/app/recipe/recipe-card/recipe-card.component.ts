import { Component, OnInit, Input } from '@angular/core';
import { RECIPES } from '../recipe.data';
import { Recipe } from '../recipe.interface';

@Component({
  selector: 'app-recipe-card',
  templateUrl: './recipe-card.component.html',
  styleUrls: ['./recipe-card.component.css'],
})
export class RecipeCardComponent implements OnInit {
  @Input() id: number = 0;
  recipe!: Recipe;

  constructor() {}

  ngOnInit(): void {
    this.recipe = RECIPES[this.id];
  }
}
